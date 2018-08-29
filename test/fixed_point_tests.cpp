#include "catch.hpp"

#include "fixed_point.h"

namespace FuncGen {

  // Q: What is the goal here?
  // A: Eventually it is to get a prototype of the signed/unsigned/fixed point
  // division unit working

  // What are the steps I'm going to take?
  // 1. Get arbitrary exponent fixed point add / sub / mul working
  // 2. Get arbitrary exponent Newton-Raphson working
  // 3. See if I can figure out how to separate exponent manipulation
  //    from the iterative update
  TEST_CASE("16 bit fixed point multiply") {

    SECTION("4*4 == 16") {
      FixedPoint a = {BitVector(1, 0), BitVector(16, 4), 0};

      auto prod = mul(a, a);
      
      SECTION("Sign has 1 bit") {
        REQUIRE(prod.sign.bitLength() == 1);
      }

      SECTION("Mantissa has 16 bits") {
        REQUIRE(prod.bits.bitLength() == 16);
      }

      SECTION("Result is correct") {
        REQUIRE(fixedPointToDouble(prod) == 16.0);
      }
    }
  }

  BitVector twos_complement_negate(const BitVector& b) {
    return add_general_width_bv(~b, BitVector(b.bitLength(), 1));
  }

  FixedPoint approximate(const FixedPoint& b) {
    return FixedPoint(0, BitVector(b.bits.bitLength(), 1 << -b.exponent), b.exponent);
  }

  BitVector newton_raphson_divide(const BitVector& NE, const BitVector& DE) {

    BitVector N = NE;
    BitVector D = DE;

    int decimalPlace = NE.bitLength() - 1;

    // If D is negative, negate and then
    if (high_bit(D) == 1) {
      D = twos_complement_negate(D);
    }

    if (high_bit(N) == 1) {
      N = twos_complement_negate(N);
    }
    
    assert(N.bitLength() == D.bitLength());
    
    int width = N.bitLength();
    FixedPoint D_(0, normalize_left(D, 1), -decimalPlace);
    int shiftDistance = num_leading_zeros(D) - 1;

    BitVector tentativeRes(width);
    if (D_.bits == BitVector(width, 1 << (width - 2))) {

      int shiftDiv = width - shiftDistance - 2;
      tentativeRes = ashr(N, BitVector(width, shiftDiv));

    } else {

      FixedPoint one = {BitVector(1, 0), BitVector(width, 1 << decimalPlace), -decimalPlace};
      FixedPoint X = approximate(D_);

      for (int i = 0; i < 5; i++) {
        X = add(X, mul(X, sub(one, mul(D_, X))));
      }

      BitVector longProd =
        mul_general_width_bv(sign_extend(2*width, N),
                             sign_extend(2*width, sign_magnitude_to_twos_complement(X.sign, X.bits)));

      tentativeRes = slice(ashr(longProd, BitVector(32, width + (width - shiftDistance) - 2)), 0, width);
    }

    if (highBit(DE) != highBit(NE)) {
      tentativeRes = twos_complement_negate(tentativeRes);
    }

    return tentativeRes;
  }

  TEST_CASE("NR test") {
    int width = 16;
    FixedPoint one = {BitVector(1, 0), BitVector(width, 1 << 15), -15};
    FixedPoint X(0, BitVector(width, 1 << 14), -15);
    cout << "one       = " << one << endl;
    cout << "X         = " << X << endl;

    SECTION("0.5 * 1") {
      auto res = mul(one, X);
      cout << "one*X     = " << res << endl;

      REQUIRE(res == X);
    }

    SECTION("1.0 - 0.5") {
      auto diff = sub(one, X);

      REQUIRE(diff == X);
    }

    SECTION("0.5 - 1.0") {
      auto diff = sub(X, one);

      cout << "Diff = " << diff << endl;
      REQUIRE(diff == flipSign(X));
    }

    SECTION("Newton raphson 20 / 3") {
      // D is initially a bitvector
      BitVector N(16, 20);
      BitVector D(16, 3);

      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 20 / 3));
    }
    
    SECTION("Newton raphson 20 / 5") {
      // D is initially a bitvector
      BitVector N(16, 20);
      BitVector D(16, 5);

      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 4));
    }

    SECTION("Newton raphson 20 / 2") {
      // D is initially a bitvector
      BitVector N(16, 20);
      BitVector D(16, 2);

      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 10));
    }
    
    SECTION("Newton raphson -20 / 5") {
      // D is initially a bitvector
      BitVector N(16, -20);
      BitVector D(16, 5);

      cout << "N = " << N << endl;
      cout << "D = " << D << endl;
      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, -4));
    }

    SECTION("Newton raphson -118 / 7") {
      // D is initially a bitvector
      BitVector N(16, -118);
      BitVector D(16, 7);

      cout << "N = " << N << endl;
      cout << "D = " << D << endl;
      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, -118 / 7));
    }
    
    SECTION("Newton raphson 14 / 7") {
      // D is initially a bitvector
      BitVector N(16, 14);
      BitVector D(16, 7);

      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 14 / 7));
    }
    
    SECTION("Newton raphson 14 / 6") {
      // D is initially a bitvector
      BitVector N(16, 14);
      BitVector D(16, 6);

      auto prod = newton_raphson_divide(N, D);
      cout << "Quotient         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 14 / 6));
    }


    SECTION("Randomized testing") {
      
      for (int i = 0; i < 100; i++) {
        cout << "Rand i = " << i << endl;
        int Ni = iRand(-1000, 1000);
        int Di = iRand(-100, 100);

        cout << "Ni = " << Ni << endl;
        cout << "Di = " << Di << endl;

        BitVector N(16, Ni);
        BitVector D(16, Di);

        cout << "N = " << N << endl;
        cout << "D = " << D << endl;

        auto prod = newton_raphson_divide(N, D);
        cout << "Quotient         = " << prod << endl;

        REQUIRE(prod == BitVector(16, Ni / Di));
      }
    }
    
  }
    

}
