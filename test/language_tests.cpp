#include "catch.hpp"

#include "language.h"
#include "simulator.h"

using namespace std;

namespace FuncGen {

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
    
    Value* result = udiv->shiftLeft(1, tempRes);

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
