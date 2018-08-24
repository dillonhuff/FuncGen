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

    auto zeroInFixedPoint = divOne->constant(width + 1, 1 << width);
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
    Function* divByOneTable = precomputeTable("one_divided_by_table", divByOne, c);

    // Replace this with table lookup
    auto fb = udiv->functionCall(divByOneTable->getName(), {{"in", udiv->getValue("b")}});
    //auto fb = udiv->tableLookup(divByOneTable, udiv->getValue("b"));

    auto res = udiv->multiply(udiv->zeroExtend(width*2, udiv->getValue("a")),
                              udiv->zeroExtend(width*2, fb));

    auto rt = udiv->logicalShiftRight(width, res);
    udiv->assign(udiv->getValue("quotient"), udiv->slice(width - 1, 0, rt));

    SECTION("10 / 3 == 3") {
      Simulator sim(*udiv);
      sim.setInput("a", BitVector(width, 10));
      sim.setInput("b", BitVector(width, 3));

      sim.evaluate();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, 3));
    }

    SECTION("129 / 8") {
      Simulator sim(*udiv);
      sim.setInput("a", BitVector(width, 129));
      sim.setInput("b", BitVector(width, 8));

      sim.evaluate();

      REQUIRE(sim.getOutput("quotient") == BitVector(width, 129 / 8));
    }

  }

  TEST_CASE("16 bit newton-raphson division") {
    int width = 16;
    Context c;
    Function* f = c.newFunction("newton_raphson_div_16",
                                {{"N", c.arrayType(width)},
                                    {"D", c.arrayType(width)}},
                                {{"quotient", c.arrayType(width)}});

    int numIterations = ceil(log2((width + 1) / log2(17)));
    cout << "Number of iterations = " << numIterations << endl;

    auto c1 = f->unsignedDivide(f->constant(width, 48),
                                f->constant(width, 17));
    auto c2 = f->unsignedDivide(f->constant(width, 32),
                                f->constant(width, 17));

    auto x = f->subtract(c1, f->multiply(c2, f->getValue("D")));

    f->repeat(numIterations,
              f->assignStmt(x,
                            f->plusExpr(x,
                                        f->timesExpr(x,
                                                     f->subExpr(f->constant(width, 1),
                                                                f->timesExpr(f->getValue("D"),
                                                                             x))))));

    auto res = f->multiply(f->getValue("N"), x);
    f->assign(f->getValue("quotient"), res);

    Simulator sim(*f);
    sim.setInput("N", BitVector(width, 10));
    sim.setInput("D", BitVector(width, 4));
    sim.evaluate();

    REQUIRE(sim.getOutput("quotient") == BitVector(width, 10 / 4));
  }
}
