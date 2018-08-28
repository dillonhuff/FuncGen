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

  TEST_CASE("NR test") {
    int width = 16;
    FixedPoint one = {BitVector(1, 0), BitVector(width, 1 << 15), -15};
    FixedPoint X(0, BitVector(width, 1 << 14), -15);
    cout << "one      = " << one << endl;
    cout << "X        = " << X << endl;
    
  }
    

}
