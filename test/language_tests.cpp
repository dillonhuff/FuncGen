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

  TEST_CASE("Fixed point divide") {
    int width = 8;

    Context c;
    Function* udiv =
      c.newFunction("fixed_point_divide_8",
                    {{"a", c.arrayType(width)}, {"b", c.arrayType(width)}},
                    {{"quotient", c.arrayType(width)}});

    Value* aExt = udiv->zeroExtend(width + 1, udiv->getValue("a"));
    Value* bExt = udiv->zeroExtend(width + 1, udiv->getValue("b"));

    Value* tempRes = udiv->unsignedDivide(aExt, bExt);
    
    //Value* result = udiv->shiftLeft(1, tempRes);
    udiv->assign(udiv->getValue("quotient"), udiv->slice(width - 1, 0, tempRes));

    Simulator sim(*udiv);
    sim.setInput("a", BitVector(width, 1));
    sim.setInput("b", BitVector(width, 2));

    sim.evaluate();

    REQUIRE(sim.getOutput("quotient") == BitVector(width, "10000000"));
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

    REQUIRE(sim.getOutput("quotient") == BitVector(width, 4));
  }
}
