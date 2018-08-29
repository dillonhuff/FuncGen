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

  BitVector newton_raphson_divide(const BitVector& N, const BitVector& D) {
    assert(N.bitLength() == D.bitLength());
    
    int width = N.bitLength();
    FixedPoint one = {BitVector(1, 0), BitVector(width, 1 << 15), -15};
    FixedPoint X(0, BitVector(width, 1 << 14), -15);
    cout << "one       = " << one << endl;
    cout << "X         = " << X << endl;
    
    // Step one normalize D
    FixedPoint D_(0, normalize_left(D, 1), -15);

    cout << "D_     =" << D_ << endl;
    cout << "1 / D_ = " << 1 / fixedPointToDouble(D_) << endl;

    // Step two refine the approximation of 1 / D
    for (int i = 0; i < 10; i++) {
      X = add(X, mul(X, sub(one, mul(D_, X))));

      cout << "X_" << i << " = " << X << ", " << fixedPointToDouble(X) << endl;
    }

    BitVector prod =
      mul_general_width_bv(N,
                           sign_magnitude_to_twos_complement(X.sign, X.bits));

    return prod;
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

    SECTION("Newton raphson 20 / 5") {
      // D is initially a bitvector
      BitVector N(16, 20);
      BitVector D(16, 5);

      auto prod = newton_raphson_divide(N, D);
      // // Step one normalize D
      // FixedPoint D_(0, normalize_left(D, 1), -15);

      // cout << "D_     =" << D_ << endl;
      // cout << "1 / D_ = " << 1 / fixedPointToDouble(D_) << endl;

      // // Step two refine the approximation of 1 / D
      // for (int i = 0; i < 10; i++) {
      //   X = add(X, mul(X, sub(one, mul(D_, X))));

      //   cout << "X_" << i << " = " << X << ", " << fixedPointToDouble(X) << endl;
      // }

      // BitVector prod =
      //   mul_general_width_bv(N,
      //                        sign_magnitude_to_twos_complement(X.sign, X.bits));
    
      cout << "Product         = " << prod << endl;

      REQUIRE(prod == BitVector(16, 4));
    }
  }
    

}
