#include "catch.hpp"

#include "language.h"
#include "simulator.h"

using namespace std;

namespace FuncGen {

  class FixedPoint {
  public:
    BitVector bits;
    int exponent;

    int bitLength() const {
      return bits.bitLength();
    }
  };

  std::ostream& operator<<(std::ostream& out, const FixedPoint& n) {
    for (int i = n.bitLength() - 1; i >= 0; i--) {
      out << n.bits.get(i);
      if (i == -n.exponent) {
        out << ".";
      }
    }
    return out;
  }
  
  FixedPoint multiply(const FixedPoint& a, const FixedPoint& b) {
    auto aBitsExt = zero_extend(2*a.bitLength(), a.bits);
    auto bBitsExt = zero_extend(2*b.bitLength(), b.bits);
    auto prodBits = mul_general_width_bv(aBitsExt, bBitsExt);
    return {prodBits, b.exponent + a.exponent};
  }

  FixedPoint leftExtend(const FixedPoint& num) {
    int lastOnePos = num.bitLength() - 1;
    while (num.bits.get(lastOnePos).binary_value() != 1) {
      lastOnePos--;
    }

    int shiftAmount = num.bitLength() - lastOnePos - 1;
    
    auto numShiftedBits = shl(num.bits, BitVector(32, shiftAmount));
    auto numShiftedExp = num.exponent - shiftAmount;

    FixedPoint shiftedNum{numShiftedBits, numShiftedExp};

    return shiftedNum;
  }

  FixedPoint rightTruncate(int toTruncate, const FixedPoint& num) {
    BitVector truncatedBits(num.bitLength() - toTruncate, 0);
    for (int i = num.bitLength() - 1; i >= toTruncate; i--) {
      truncatedBits.set(i - toTruncate, num.bits.get(i));
    }
    return {truncatedBits, num.exponent + toTruncate};
  }

  // Give max precision within range
  FixedPoint divide(const FixedPoint& numE, const FixedPoint& denomE) {
    auto num = FixedPoint{zero_extend(numE.bitLength()*2, numE.bits), numE.exponent};
    auto denom = FixedPoint{zero_extend(denomE.bitLength()*2, denomE.bits), denomE.exponent};

    cout << endl;
    cout << "numerator   = " << num << endl;
    cout << "denominator = " << denom << endl;

    FixedPoint shiftedNum = leftExtend(num);
    // int lastOnePos = num.bitLength() - 1;
    // while (num.bits.get(lastOnePos).binary_value() != 1) {
    //   lastOnePos--;
    // }

    // cout << "last one position in " << num << " is " << lastOnePos << endl;
    // int shiftAmount = num.bitLength() - lastOnePos - 1;
    
    // auto numShiftedBits = shl(num.bits, BitVector(32, shiftAmount));
    // auto numShiftedExp = num.exponent - shiftAmount;

    // FixedPoint shiftedNum{numShiftedBits, numShiftedExp};
    cout << "Shifted num = " << shiftedNum << endl;

    // auto prodBits = mul_general_width_bv(aBitsExt, bBitsExt);
    // return {prodBits, b.exponent + a.exponent};

    return {unsigned_divide(shiftedNum.bits, denom.bits), shiftedNum.exponent - denom.exponent};
  }
  
  FixedPoint add(const FixedPoint& a, const FixedPoint& b) {
    assert(false);
  }

  FixedPoint renormalizeExponent(const int newExp, const FixedPoint& bE) {
    int diff = newExp - bE.exponent;

    if (diff == 0) {
      return bE;
    }

    if (diff > 0) {
      return {shl(bE.bits, BitVector(32, diff)), newExp};
    }

    return {lshr(bE.bits, BitVector(32, diff)), newExp};

  }

  FixedPoint subtract(const FixedPoint& aE, const FixedPoint& bE) {
    auto a = leftExtend(aE);
    auto b = renormalizeExponent(a.exponent, bE);

    cout << "aE = " << aE << endl;
    cout << "bE = " << bE << endl;
    
    cout << "a  = " << a << endl;
    cout << "b  = " << b << endl;
    //auto b = leftExtend(bE);

    
    assert(false);
  }
  
  double approximateFixedPoint(const BitVector& bv, int binaryPlacePosition) {
    double approx = 0;
    for (int i = 0; i < (int) bv.bitLength(); i++) {
      if (i >= binaryPlacePosition) {
        approx += pow(2, i - binaryPlacePosition)*bv.get(i).binary_value();
      } else {
        approx += pow(2, -(binaryPlacePosition - i))*bv.get(i).binary_value();
      }
    }

    return approx;
  }

  double fixedPointToDouble(const FixedPoint& fp) {
    return approximateFixedPoint(fp.bits, -fp.exponent);
  }

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

  BitVector doubleToBV(const double d) {
    BitVector res(64, 0);

    unsigned long long* dptr = (unsigned long long*) (&d);

    for (int i = 0; i < 64; i++) {
      res.set(i, quad_value((*dptr >> i) & 0b1));
    }

    return res;
  }

  double bvToDouble(const BitVector& b) {
    assert(b.bitLength() == 64);

    double res = 0;
    for (int i = 0; i < 64; i++) {
      *((unsigned long long*) (&res)) |= (((unsigned long long) b.get(i).binary_value()) << i);
    }

    return res;
  }

  BitVector get_double_exponent(const BitVector& a) {
    assert(a.bitLength() == 64);
    return slice(a, 52, 63);
  }

  BitVector get_double_mantissa(const BitVector& a) {
    assert(a.bitLength() == 64);
    return slice(a, 0, 52);
  }
  
  BitVector get_double_sign_bit(const BitVector& a) {
    assert(a.bitLength() == 64);

    return BitVector(1, a.get(63).binary_value());
  }

  BitVector double_float_multiply(const BitVector& a, const BitVector& b) {
    BitVector sgnA = get_double_sign_bit(a);
    BitVector expA = get_double_exponent(a);
    BitVector sigA = get_double_mantissa(a);

    assert(expA.bitLength() == 11);
    assert(sigA.bitLength() == 52);

    BitVector sgnB = get_double_sign_bit(b);
    BitVector expB = get_double_exponent(b);
    BitVector sigB = get_double_mantissa(b);

    assert(expB.bitLength() == 11);
    assert(sigB.bitLength() == 52);

    const int DOUBLE_BIAS = 1023;

    BitVector tentativeExp =
      sub_general_width_bv(add_general_width_bv(expA, expB),
                           BitVector(11, DOUBLE_BIAS));

    cout << "tentativeExp = " << tentativeExp << ", int = " << tentativeExp.to_type<int>() << endl;

    // Extend precision and set implicit bit
    BitVector sigAExt = zero_extend(2*(sigA.bitLength() + 1), sigA);
    sigAExt.set(sigA.bitLength(), 1);
    BitVector sigBExt = zero_extend(2*(sigB.bitLength() + 1), sigB);
    sigBExt.set(sigB.bitLength(), 1);

    BitVector sigProd = mul_general_width_bv(sigAExt, sigBExt);
    cout << "sigProd = " << sigProd << endl;

    if (sigProd.get(sigProd.bitLength() - 1).binary_value() == 1) {
      sigProd = lshr(sigProd, BitVector(32, 1));
      tentativeExp = add_general_width_bv(tentativeExp, BitVector(11, 1));
    }

    auto M0 = sigProd.get(51).binary_value();
    auto R = sigProd.get(50).binary_value();
    auto S = orr(slice(sigProd, 0, 50)).get(0).binary_value();

    if (R*(M0 + S) != 0) {
      BitVector roundOne(52*2, 0);
      cout << "Rounding" << endl;
      roundOne.set(51, 1);

      assert(roundOne.bitLength() == sigProd.bitLength());

      sigProd = add_general_width_bv(sigProd, roundOne);
    }

    cout << "Normalized sigProd = " << sigProd << endl;

    // TODO: Add re-normalization!!!!

    BitVector sgnR(1, 0);
    if (sgnA == sgnB) {
      sgnR.set(0, 0);
    } else {
      sgnR.set(0, 1);
    }

    BitVector result(64, 0);
    result.set(63, sgnR.get(0));
    for (int i = 0; i < tentativeExp.bitLength(); i++) {
      result.set(52 + i, tentativeExp.get(i));
    }

    cout << "Sigprod width = " << sigProd.bitLength() << endl;

    //for (int i = 0; i < (sigProd.bitLength() / 2) - 1; i++) {
    for (int i = 0; i < 52; i++) {
      result.set(i, sigProd.get(i + 52));
    }
    
    return result;
  }

  TEST_CASE("Floating point multiply") {
    int width = 64;
    int mantissaWidth = 52;
    int exponentWidth = 11;

    REQUIRE((1 + mantissaWidth + exponentWidth) == width);

    REQUIRE(bvToDouble(doubleToBV(123.5)) == 123.5);

    REQUIRE(bvToDouble(doubleToBV(1)) == 1);

    REQUIRE(bvToDouble(doubleToBV(39201.455)) == 39201.455);

    BitVector one = doubleToBV(1);
    BitVector two = doubleToBV(2);

    cout << "One as double = " << one << endl;
    cout << "Two as double = " << two << endl;

    BitVector product = double_float_multiply(one, two);
    REQUIRE(bvToDouble(product) == (1.0*2.0));

    product = double_float_multiply(doubleToBV(123.7), doubleToBV(13.4));

    double correct = (123.7*13.4);
    cout.precision(17);
    cout << "Product   = " << bvToDouble(product) << endl;
    cout << "Correct   = " << correct << endl;

    cout << "ProductBV = " << doubleToBV(bvToDouble(product)) << endl;
    cout << "CorrectBV = " << doubleToBV(correct) << endl;

    REQUIRE(bvToDouble(product) == correct);

  }

  // TEST_CASE("8 bit newton raphson experiment") {

  //   FixedPoint f10{BitVector(8, 10), 0};
  //   FixedPoint f3{BitVector(8, 3), 0};

  //   FixedPoint f48{BitVector(8, 48), 0};
  //   FixedPoint f17{BitVector(8, 17), 0};

  //   auto quot = divide(f48, f17);
  //   cout << "48 / 17 = " << divide(f48, f17) << endl;
  //   cout << "quot*17 = " << fixedPointToDouble(quot)*17 << endl;

  //   FixedPoint c0 = rightTruncate(quot.bitLength() / 2, leftExtend(quot));
  //   cout << "left extended quotient = " << c0 << endl;

  //   FixedPoint f32{BitVector(8, 32), 0};
  //   auto c1 = rightTruncate(quot.bitLength() / 2, leftExtend(divide(f32, f17)));
  //   cout << "left extended quotient = " << c1 << endl;

  //   auto D = f3;
  //   auto X = subtract(c0, rightTruncate(quot.bitLength(), leftExtend(multiply(c1, D))));

  //   cout << "X = " << X << endl;
    
  //   assert(false);
  // }

  // TEST_CASE("16 bit newton-raphson division") {
  //   int width = 16;
  //   Context c;
  //   Function* f = c.newFunction("newton_raphson_div_16",
  //                               {{"N", c.arrayType(width)},
  //                                   {"D", c.arrayType(width)}},
  //                               {{"quotient", c.arrayType(width)}});

  //   int numIterations = ceil(log2((width + 1) / log2(17)));
  //   cout << "Number of iterations = " << numIterations << endl;

  //   auto c1 = f->unsignedDivide(f->constant(width, 48),
  //                               f->constant(width, 17));
  //   auto c2 = f->unsignedDivide(f->constant(width, 32),
  //                               f->constant(width, 17));

  //   BitVector bv(width, 32);
  //   BitVector bv0(width, 17);

  //   cout << "32 = " << bv << endl;
  //   cout << "17 = " << bv0 << endl;
  //   auto quot = unsigned_divide(shl(bv, BitVector(32, 10)), bv0);
  //   double approx = approximateFixedPoint(quot, 10);
  //   cout << "Quotient c2 = " << quot << ", fixed point = " << approx << ", bv0*approx = " << bv0.to_type<int>()*approx << endl;

  //   assert(false);
    
  //   auto x = f->zeroExtend(2*width,
  //                          f->subtract(c1, f->multiply(c2, f->getValue("D"))));

  //   auto dext = f->zeroExtend(2*width, f->getValue("D"));

  //   f->repeat(numIterations,
  //             f->assignStmt(x,
  //                           f->plusExpr(x,
  //                                       f->timesExpr(x,
  //                                                    f->subExpr(f->constant(2*width, 1),
  //                                                               f->timesExpr(dext,
  //                                                                            x))))));

    
  //   auto res = f->multiply(f->getValue("N"), x);
  //   f->assign(f->getValue("quotient"), res);

  //   Simulator sim(*f);
  //   sim.setInput("N", BitVector(width, 10));
  //   sim.setInput("D", BitVector(width, 4));
  //   sim.evaluate();

  //   REQUIRE(sim.getOutput("quotient") == BitVector(width, 10 / 4));
  // }
}
