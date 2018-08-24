#include "catch.hpp"

#include "language.h"
#include "simulator.h"

using namespace std;

namespace FuncGen {

  TEST_CASE("Zero extend") {
    int width = 8;

    Context c;
    Function* udiv =
      c.newFunction("zero_extend_by_5",
                    {{"a", c.arrayType(width)}},
                    {{"res", c.arrayType(width + 5)}});

    Value* aExt = udiv->zeroExtend(width + 5, udiv->getValue("a"));
    udiv->assign(udiv->getValue("res"), aExt);

    Simulator sim(*udiv);
    sim.setInput("a", BitVector(width, 1));

    sim.evaluate();

    REQUIRE(sim.getOutput("res") == BitVector(width + 5, "0000000000001"));
  }

  TEST_CASE("Integer division with shifting and slicing") {
    int width = 8;

    Context c;
    Function* udiv =
      c.newFunction("fixed_point_divide_8",
                    {{"a", c.arrayType(width)}, {"b", c.arrayType(width)}},
                    {{"quotient", c.arrayType(width)}});

    Value* aExt = udiv->zeroExtend(width + 1, udiv->getValue("a"));
    Value* bExt = udiv->zeroExtend(width + 1, udiv->getValue("b"));

    Value* tempRes = udiv->unsignedDivide(aExt, bExt);
    
    auto sliceRes = udiv->slice(width - 1, 0, tempRes);

    udiv->assign(udiv->getValue("quotient"), sliceRes);

    SECTION("8 divided by 2") {
      Simulator sim(*udiv);
      sim.setInput("a", BitVector(width, 8));
      sim.setInput("b", BitVector(width, 2));

      sim.evaluate();

      sim.printValueStore();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, "00000100"));
    }

    SECTION("19 divided by 5") {
      Simulator sim(*udiv);
      sim.setInput("a", BitVector(width, 19));
      sim.setInput("b", BitVector(width, 5));

      sim.evaluate();

      sim.printValueStore();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, 19 / 5));
    }

  }

  Function* buildDivideByOne(const int width, Context& c) {
    Function* divOne =
      c.newFunction("divide_one_by_x_" + std::to_string(width),
                    {{"x", c.arrayType(width)}},
                    {{"quotient", c.arrayType(width)}});

    // Q: What is the process?
    // 1. Create value one constant of width (w + 1), then shift so that the
    //    one is in the last place.
    // 2. Zero extend the input
    // 3. Do unsigned division
    // 4. Slice away the last bit

    auto zeroInFixedPoint = divOne->addConstant(width + 1, 1 << width);
    auto divisor = divOne->zeroExtend(width + 1, divOne->getValue("x"));
    auto initQuotient = divOne->unsignedDivide(zeroInFixedPoint, divisor);
    auto finalQuote = divOne->slice(width - 1, 0, initQuotient);
    divOne->assign(divOne->getValue("quotient"), finalQuote);

    return divOne;
  }

  TEST_CASE("Fixed point one divided by x") {
    int width = 8;
    Context c;
    Function* divOne = buildDivideByOne(width, c);

    SECTION("1/2 == 0.5") {
      Simulator sim(*divOne);
      sim.setInput("x", BitVector(width, 2));

      sim.evaluate();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, "10000000"));
    }

    SECTION("1/3 == 0.3...") {
      Simulator sim(*divOne);
      sim.setInput("x", BitVector(width, 3));

      sim.evaluate();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, "01010101"));
    }

  }

  TEST_CASE("Lookup table based integer division") {
    int width = 8;

    Context c;
    Function* udiv =
      c.newFunction("lookup_based_unsigned_divide_8",
                    {{"a", c.arrayType(width)}, {"b", c.arrayType(width)}},
                    {{"quotient", c.arrayType(width)}});

    // Implementation?
    // 1. Compute f(b) = 1/b from a table of existing values
    // 2. Compute a*f(b)
    // 3. Shift a to the right width places
    // 4. Correct for special cases like b == 1

    // Critical Issue: Pre-computed tables. How do I express this concept?
    Function* divByOne = buildDivideByOne(width, c);
    auto fb = udiv->functionCall(divByOne->getName(), {{"x", udiv->getValue("b")}});

    Simulator sim(*udiv);
    sim.setInput("a", BitVector(width, 10));
    sim.setInput("b", BitVector(width, 3));

    sim.evaluate();

    REQUIRE(sim.getOutput("quotient") == BitVector(width, 3));
  }
}
