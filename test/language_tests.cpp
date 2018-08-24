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

  TEST_CASE("Lookup table based division") {
    int width = 8;

    Context c;
    Function* udiv =
      c.newFunction("lookup_based_unsigned_divide_8",
                    {{"a", c.arrayType(width)}, {"b", c.arrayType(width)}},
                    {{"quotient", c.arrayType(width)}});

    Simulator sim(*udiv);
    sim.setInput("a", BitVector(width, 8));
    sim.setInput("b", BitVector(width, 2));

    sim.evaluate();

    //REQUIRE(sim.getOutput("quotient") == BitVector(width, 4));
  }
}
