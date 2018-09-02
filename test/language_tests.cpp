#include "catch.hpp"

#include "fixed_point.h"
#include "language.h"
#include "simulator.h"
#include "synthesis.h"

using namespace std;

namespace FuncGen {

  // To make things easier this macro set needs to:
  // 1. Allow hieararchical changes of statement that things are being
  //    appended to
  // 2. Allow expressions to be safely allocated somewhere
  // 3. Allow transitions in and out of 

  Function* active_function;
  
  Statement* active_stmt_block;
  Statement* last_active_stmt_block;

#define IN_STMT(f, body) { last_active_stmt_block = active_stmt_block; \
    active_stmt_block = (f);                                              \
    (body)                                                              \
      active_stmt_block = last_active_stmt_block;                           \
    last_active_stmt_block = nullptr; }

#define IN_FUNC(f, body) {active_function = (f); (body); }
#define SET(a, b) auto a = active_function->freshVal(#a, b->bitWidth()); active_function->assign(a, b);

#define BUILD_FUNC(f) {active_function = (f); }

#define IF(cond, a, b) { aBlock = new Block(); IN_STMT(aBlock, (a)); bBlock = new Block(); IN_STMT(bBlock, (b)); add_if_to_active_statement(cond, ablock, bblock); }

  // IF(a,
  //    SET(a, b)
  //    SET(a, b + 1),
  //    SET(a, 0))

  // Becomes something like
  // append new if statement to current statement block
  // set that if's true condition to be the active statement block
  // add statement SET(a, b) to active block
  // add statement SET(a, b + 1) to active block
  //

  bool runCmd(const std::string& cmd) {
    bool res = system(cmd.c_str());
    return res == 0;
  }

  bool runVerilatorTB(const std::string& moduleName) {

    string mainName = moduleName + "_tb.cpp";

    string genCmd = "verilator -Wall -Wno-DECLFILENAME --cc " + moduleName + ".v --exe " + mainName + " --top-module " + moduleName + " -CFLAGS -std=c++14";
    
bool genVerilator = runCmd(genCmd);
    if (!genVerilator) {
      return false;
    }

    string makeCmd = "make -C obj_dir -j -f V" + moduleName + ".mk V" + moduleName;
    bool makeTb = runCmd(makeCmd);
    if (!makeTb) {
      return false;
    }

    string exeCmd = "./obj_dir/V" + moduleName;
    bool runTb = runCmd(exeCmd);

    return runTb;
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

  const int DOUBLE_WIDTH = 64;
  const int DOUBLE_EXP_WIDTH = 11;
  const int DOUBLE_MANTISSA_WIDTH = 52;
  const int DOUBLE_BIAS = 1023;
  
  BitVector normalizeDoubleSubnormal(const BitVector& a) {
    BitVector sgnA = get_double_sign_bit(a);
    BitVector expA = get_double_exponent(a);
    BitVector sigA = get_double_mantissa(a);

    BitVector toShift =
      sub_general_width_bv(BitVector(DOUBLE_EXP_WIDTH, num_leading_zeros(sigA)), BitVector(DOUBLE_EXP_WIDTH, 11));

    cout << "toShift = " << toShift << endl;

    BitVector mantissa = shl(sigA, toShift);
    BitVector exp =
      sub_general_width_bv(BitVector(DOUBLE_EXP_WIDTH, 1), toShift);

    BitVector result(DOUBLE_WIDTH, 0);

    result.set(63, a.get(63));

    for (int i = 0; i < DOUBLE_MANTISSA_WIDTH; i++) {
      result.set(i, mantissa.get(i));
    }

    for (int i = DOUBLE_MANTISSA_WIDTH; i < DOUBLE_MANTISSA_WIDTH + DOUBLE_EXP_WIDTH; i++) {
      result.set(i, exp.get(i - DOUBLE_MANTISSA_WIDTH));
    }
    
    return result;
    //assert(false);
  }

  BitVector double_float_add(const BitVector& a, const BitVector& b) {
    BitVector sgnA = get_double_sign_bit(a);
    auto expA = get_double_exponent(a);
    BitVector sigA = get_double_mantissa(a);

    assert(expA.bitLength() == 11);
    assert(sigA.bitLength() == 52);

    BitVector sgnB = get_double_sign_bit(b);
    auto expB = get_double_exponent(b);
    BitVector sigB = get_double_mantissa(b);

    assert(expB.bitLength() == 11);
    assert(sigB.bitLength() == 52);

    BitVector tentativeExp = expA;
    BitVector smallerExp = expB;

    BitVector largerSig = sigA;
    BitVector smallerSig = sigB;

    BitVector largerSgn = sgnA;
    BitVector smallerSgn = sgnB;
    
    BitVector larger = a;
    BitVector smaller = b;

    bool aLarger = true;
    if (expB > expA) {
      aLarger = false;
      tentativeExp = expB;
      smallerExp = expA;

      largerSgn = sgnB;
      smallerSgn = sgnA;

      larger = b;
      largerSig = sigB;
      smaller = a;
      smallerSig = sigA;
    }

    cout << "Smaller = " << smaller << endl;
    cout << "Larger  = " << larger << endl;

    BitVector expDiff = sub_general_width_bv(tentativeExp, smallerExp);

    BitVector smallerOp(smallerSig.bitLength() + 4, 0);
    smallerOp = shl(zero_extend(smallerSig.bitLength() + 4, smallerSig),
                    BitVector(32, 3));

    cout << "Smaller op 1 = " << smallerOp << endl;

    // Set implicit leading one
    smallerOp.set(smallerOp.bitLength() - 1, 1);

    // Shift operand over so that it is aligned with the larger operand
    cout << "tenative exp = " << tentativeExp << endl;
    cout << "smaller exp  = " << smallerExp << endl;
    cout << "expDiff      = " << expDiff << endl;

    smallerOp = lshr(smallerOp, expDiff);

    // Set the sticky bit
    smallerOp.set(0, orr(slice(smallerSig, 0, expDiff.to_type<int>())).get(0));

    cout << "Smaller op = " << smallerOp << endl;
    
    BitVector largerOp(largerSig.bitLength() + 4, 0);
    largerOp = shl(zero_extend(largerSig.bitLength() + 4, largerSig), BitVector(32, 3));
    // Set implicit leading zero
    largerOp.set(largerOp.bitLength() - 1, 1);


    cout << "Larger sig = " << largerSig << endl;
    cout << "Larger op  = " << largerOp << endl;

    BitVector tempRes(largerOp.bitLength(), 0);
    if (sgnA == sgnB) {
      cout << "Signs match" << endl;

      tempRes =
        add_general_width_bv(zero_extend(largerOp.bitLength() + 1, largerOp),
                             zero_extend(smallerOp.bitLength() + 1, smallerOp));
    } else {

      cout << "Does a have a larger exponent than b ? " << aLarger << endl;
      if (aLarger) {
        //assert(false);
        // tempRes =
        //   sub_general_width_bv(zero_extend(largerOp.bitLength() + 1, largerOp),
        //                        zero_extend(smallerOp.bitLength() + 1, smallerOp));

        tempRes =
          sub_general_width_bv(zero_extend(smallerOp.bitLength() + 1, smallerOp),
                               zero_extend(largerOp.bitLength() + 1, largerOp));

      } else {
        tempRes =
          sub_general_width_bv(zero_extend(largerOp.bitLength() + 1, largerOp),
                               zero_extend(smallerOp.bitLength() + 1, smallerOp));

      }

      // tempRes =
      //   sub_general_width_bv(largerOp,
      //                        smallerOp);
      // tempRes = zero_extend(tempRes.bitLength() + 1, tempRes);
      
      // if (aLarger) {
      //   cout << "A is larger, doing a - b" << endl;
      //   tempRes =
      //     sub_general_width_bv(zero_extend(largerOp.bitLength() + 1, largerOp),
      //                          zero_extend(smallerOp.bitLength() + 1, smallerOp));
      // } else {
      //   cout <<" A is smaller exp" << endl;
      //   tempRes =
      //     sub_general_width_bv(zero_extend(smallerOp.bitLength() + 1, smallerOp),
      //                          zero_extend(largerOp.bitLength() + 1, largerOp));
      // }

      BitVector leadZeros =
        BitVector(32, num_leading_zeros(tempRes));
      tempRes = shl(tempRes, leadZeros);
      tentativeExp = sub_general_width_bv(tentativeExp, leadZeros);
    }

    cout << "temp res = " << tempRes << endl;

    // Normalize
    if (tempRes.get(tempRes.bitLength() - 1) == 1) {
      tempRes = lshr(tempRes, BitVector(32, 1));
      tentativeExp = add_general_width_bv(tentativeExp, BitVector(tentativeExp.bitLength(), 1));
    }

    auto M0 = tempRes.get(3).binary_value();
    auto R = tempRes.get(2).binary_value();
    auto S = orr(slice(tempRes, 0, 2)).get(0).binary_value();

    if (R*(M0 + S) != 0) {
      cout << "Rounding" << endl;
      tempRes = add_general_width_bv(tempRes,
                                     shl(BitVector(tempRes.bitLength(), 1),
                                         BitVector(32, 2)));
    }

    // Renormalize
    if (tempRes.get(tempRes.bitLength() - 1) == 1) {
      tempRes = lshr(tempRes, BitVector(32, 1));
      tentativeExp = add_general_width_bv(tentativeExp, BitVector(tentativeExp.bitLength(), 1));
    }

    BitVector mantRes = slice(tempRes, 3, DOUBLE_MANTISSA_WIDTH + 3);

    assert(mantRes.bitLength() == DOUBLE_MANTISSA_WIDTH);

    cout << "mantRes = " << mantRes << endl;

    // Set result
    BitVector result(DOUBLE_WIDTH, 0);
    if (sgnA == sgnB) {
      result.set(DOUBLE_WIDTH - 1, sgnA.get(0));
    } else {
      result.set(DOUBLE_WIDTH - 1, largerSgn.get(0));
    }
    for (int i = 0; i < DOUBLE_MANTISSA_WIDTH; i++) {
      result.set(i, mantRes.get(i));
    }

    for (int i = DOUBLE_MANTISSA_WIDTH; i < DOUBLE_MANTISSA_WIDTH + DOUBLE_EXP_WIDTH; i++) {
      result.set(i, tentativeExp.get(i - DOUBLE_MANTISSA_WIDTH));
    }
    
    return result;
  }

  BitVector double_float_multiply(const BitVector& aE, const BitVector& bE) {
    BitVector a = aE;
    BitVector b = bE;

    BitVector expA = get_double_exponent(a);
    if (expA == BitVector(11, 0)) {
      assert(false);
      //a = normalizeDoubleSubnormal(a);
    }

    BitVector expB = get_double_exponent(b);
    if (expB == BitVector(11, 0)) {
      assert(false);
      //b = normalizeDoubleSubnormal(b);
    }
    
    BitVector sgnA = get_double_sign_bit(a);
    expA = get_double_exponent(a);
    BitVector sigA = get_double_mantissa(a);

    assert(expA.bitLength() == 11);
    assert(sigA.bitLength() == 52);

    BitVector sgnB = get_double_sign_bit(b);
    expB = get_double_exponent(b);
    BitVector sigB = get_double_mantissa(b);

    assert(expB.bitLength() == 11);
    assert(sigB.bitLength() == 52);

    const int DOUBLE_BIAS = 1023;

    BitVector tentativeExp =
      sub_general_width_bv(add_general_width_bv(expA, expB),
                           BitVector(11, DOUBLE_BIAS));

    //cout << "tentativeExp = " << tentativeExp << ", int = " << tentativeExp.to_type<int>() << endl;

    // Extend precision and set implicit bit
    BitVector sigAExt = zero_extend(2*(sigA.bitLength() + 1), sigA);
    sigAExt.set(sigA.bitLength(), 1);
    BitVector sigBExt = zero_extend(2*(sigB.bitLength() + 1), sigB);
    sigBExt.set(sigB.bitLength(), 1);

    BitVector sigProd = mul_general_width_bv(sigAExt, sigBExt);
    //cout << "sigProd = " << sigProd << endl;

    if (sigProd.get(sigProd.bitLength() - 1).binary_value() == 1) {
      sigProd = lshr(sigProd, BitVector(32, 1));
      tentativeExp = add_general_width_bv(tentativeExp, BitVector(11, 1));
    }

    auto M0 = sigProd.get(52).binary_value();
    auto R = sigProd.get(51).binary_value();
    auto S = orr(slice(sigProd, 0, 51)).get(0).binary_value();

    if (R*((M0 + S)) != 0) {
      BitVector roundOne(53*2, 0);
      //cout << "Rounding" << endl;
      roundOne.set(52, 1);

      assert(roundOne.bitLength() == sigProd.bitLength());

      sigProd = add_general_width_bv(sigProd, roundOne);
    }

    //cout << "Normalized sigProd = " << sigProd << endl;

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

    //cout << "Sigprod width = " << sigProd.bitLength() << endl;

    for (int i = 0; i < 52; i++) {
      result.set(i, sigProd.get(i + 52));
    }
    
    return result;
  }

  TEST_CASE("Double precision floating point add") {
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

    BitVector product = double_float_add(one, two);
    REQUIRE(bvToDouble(product) == (1.0+2.0));

    product = double_float_add(doubleToBV(123.7), doubleToBV(13.4));

    double correct = (123.7 + 13.4);
    cout.precision(17);
    cout << "Sum       = " << bvToDouble(product) << endl;
    cout << "Correct   = " << correct << endl;

    cout << "SumBV     = " << doubleToBV(bvToDouble(product)) << endl;
    cout << "CorrectBV = " << doubleToBV(correct) << endl;

    REQUIRE(bvToDouble(product) == correct);

    // cout << "Random testing" << endl;
    // for (int i = 0; i < 1000; i++) {
    //   double a = fRand(-100, 100);
    //   double b = fRand(-100, 100);
    //   double correct = (a + b);

    //   cout << "a = " << a << endl;
    //   cout << "b = " << b << endl;
      
    //   BitVector sum = double_float_add(doubleToBV(a), doubleToBV(b));

    //   cout << "SumBV     = " << doubleToBV(bvToDouble(sum)) << endl;
    //   cout << "CorrectBV = " << doubleToBV(correct) << endl;
      
    //   cout << "SumBV     = " << bvToDouble(sum) << endl;
    //   cout << "CorrectBV = " << correct << endl;
      
    //   REQUIRE(bvToDouble(sum) == correct);
    // }
  }

  TEST_CASE("Double precision floating point multiply") {
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

    cout << "Random testing" << endl;
    for (int i = 0; i < 1000; i++) {
      double a = fRand(-100, 100);
      double b = fRand(-100, 100);
      double correct = (a*b);

      product = double_float_multiply(doubleToBV(a), doubleToBV(b));

      // cout << "ProductBV = " << doubleToBV(bvToDouble(product)) << endl;
      // cout << "CorrectBV = " << doubleToBV(correct) << endl;
      
      // cout << "ProductBV = " << bvToDouble(product) << endl;
      // cout << "CorrectBV = " << correct << endl;
      
      REQUIRE(bvToDouble(product) == correct);
    }

    // cout << "Random testing of denormals" << endl;
    // for (int i = 0; i < 1000; i++) {
    //   BitVector aExp(11, 0);
    //   BitVector bExp(11, 0);

    //   BitVector aMant(52, iRand(-1000000, 1000000));
    //   BitVector bMant(52, iRand(-1000000, 1000000));

    //   BitVector aSign(1, iRand(-2, 2));
    //   BitVector bSign(1, iRand(-2, 2));

    //   BitVector a(64, 0);
    //   a.set(63, aSign.get(0));

    //   BitVector b(64, 0);
    //   b.set(63, bSign.get(0));

    //   double correct = (bvToDouble(a)*bvToDouble(b));

    //   product = double_float_multiply(a, b);

    //   cout << "ProductBV = " << doubleToBV(bvToDouble(product)) << endl;
    //   cout << "CorrectBV = " << doubleToBV(correct) << endl;
      
    //   cout << "ProductBV = " << bvToDouble(product) << endl;
    //   cout << "CorrectBV = " << correct << endl;
      
    //   REQUIRE(bvToDouble(product) == correct);
    // }

  }

  Function* buildTCNeg(const int width, Context& c) {
    auto f = c.newUnaryFunction("tc_neg_" + to_string(width),
                                "in", c.arrayType(width),
                                "out", c.arrayType(width));

    cout << "starting" << endl;
    auto in = f->getValue("in");

    auto out = f->getValue("out");
    auto invIn = f->invert(in);

    cout << "Computing res in tc neg" << endl;
    
    auto res = f->add(invIn, f->constant(width, 1));

    cout << "Assigned res" << endl;

    f->assign(out, res);

    cout << "don" << endl;

    return f;
  }


  TEST_CASE("16 bit Newton Raphson") {
    int width = 16;

    Context c;

    auto tcNegW = buildTCNeg(width, c);

    auto abs = c.newFunction("twos_complement_absolute_value",
                             {{"in", c.arrayType(width)}},
                             {{"out", c.arrayType(width)}});
    auto in = abs->getValue("in");
    auto out = abs->getValue("out");

    auto topBit = abs->slice(width - 1, width - 1, in);
    Cases absCases{{BitVector(1, 0), in}, {BitVector(1, 1), abs->unop(tcNegW, in)}};
    auto caseRes = abs->caseStatement(topBit, absCases);
    abs->assign(out, caseRes);

    SECTION("Twos complement absolute value") {

      SECTION("Positive to positive") {
        Simulator sim(*abs);
        sim.setInput("in", BitVector(16, 3));
        sim.evaluate();

        REQUIRE(sim.getOutput("out") == BitVector(16, 3));
      }

      SECTION("Negative to positive") {
        Simulator sim(*abs);
        sim.setInput("in", tc_neg(BitVector(16, 3)));
        sim.evaluate();

        REQUIRE(sim.getOutput("out") == BitVector(16, 3));
      }

    }

    auto f = c.newFunction("newton_raphson_divide_" + to_string(width),
                           {{"N", c.arrayType(width)}, {"D", c.arrayType(width)}},
                           {{"Q", c.arrayType(width)}});

    BUILD_FUNC(f);

    auto N = f->getValue("N");
    auto D = f->getValue("D");
    auto Q = f->getValue("Q");

    SET(absN, f->unop(abs, N));
    SET(absD, f->unop(abs, D));

    SET(NisNeg, f->slice(width - 1, width - 1, N));
    SET(DisNeg, f->slice(width - 1, width - 1, D));
    
    SET(DleadingZeros, f->leadZeroCount(absD)); // Priority encoder

    SET(shiftDistance, f->subtract(DleadingZeros, f->constant(width, 1)));

    SET(D_, f->shiftLeftVariable(absD, shiftDistance)); // Barrell shift

    SET(oneConst, f->constant(BitVector(width, 1 << (width - 2))));

    SET(D_isPowOfTwo, f->equals(D_, oneConst));

    SET(shiftDiv0, f->subtract(f->constant(width, width), shiftDistance));
    SET(shiftDiv, f->subtract(shiftDiv0, f->constant(width, 2)));
    SET(shrD, f->logicalShiftRightVariable(absN, shiftDiv));

    // Otherwise compute the NR update
    SET(one, f->constant(width, 1 << (width - 1)));
    SET(X, f->constant(width, 1 << (width - 1)));

    int decPlace = width - 1;
    Expression* update =
      f->plusExpr(X,
                  f->fpMul(X, f->subExpr(one, f->fpMul(D_, X, decPlace)), decPlace));
    f->repeat(4, f->assignStmt(X, update));

    SET(longProd, f->timesExpr(f->zextExpr(absN, 2*width), f->zextExpr(X, 2*width)));

    // Compute tentativ res using multiplication
    SET(wConst, f->constant(width, width));
    SET(twoConst, f->constant(width, 2));
    SET(resShift, f->plusExpr(wConst, f->subExpr(f->subExpr(wConst, shiftDistance), twoConst)));
        
    SET(a0, f->logicalShiftRightVariable(longProd, resShift));

    // TODO: Set this to be the product
    SET(tentativeRes, f->sliceExpr(width - 1, 0, a0));
    
    Cases inCases{{BitVector(1, 0), tentativeRes}, {BitVector(1, 1), shrD}};
    auto res = f->caseStatement(D_isPowOfTwo, inCases);

    SET(signsMatch, f->equals(DisNeg, NisNeg));
    Cases negCases{{BitVector(1, 0), f->unop(tcNegW, res)}, {BitVector(1, 1), res}};
    SET(finalRes, f->caseStatement(signsMatch, negCases));
    
    f->assign(Q, finalRes);

    // TODO: Negate if output is negative
    //       Do real table lookup approximation
    //       Synthesize verilog from this data structure

    // Cleanup: Create ite expression builder
    //          Move to define_function based system to reduce boilerplate
    //          Separate expression building from statement building even more

    SECTION("8 / 4 == 2") {
      Simulator sim(*f);
      sim.setInput("N", BitVector(16, 8));
      sim.setInput("D", BitVector(16, 4));
      sim.evaluate();

      REQUIRE(sim.getOutput("Q") == BitVector(16, 8 / 4));
    }

    SECTION("8 / 3 == 2") {
      Simulator sim(*f);
      sim.setInput("N", BitVector(16, 8));
      sim.setInput("D", BitVector(16, 3));
      sim.evaluate();

      REQUIRE(sim.getOutput("Q") == BitVector(16, 8 / 3));
    }

    SECTION("-8 / 3 == -2") {
      Simulator sim(*f);
      sim.setInput("N", BitVector(16, -8));
      sim.setInput("D", BitVector(16, 3));
      sim.evaluate();

      REQUIRE(sim.getOutput("Q") == BitVector(16, -(8 / 3)));
    }

    SECTION("8 / -3 == -2") {
      Simulator sim(*f);
      sim.setInput("N", BitVector(16, 8));
      sim.setInput("D", BitVector(16, -3));
      sim.evaluate();

      REQUIRE(sim.getOutput("Q") == BitVector(16, 8 / -3));
    }

    SECTION("Randomized testing") {
      
      for (int i = 0; i < 100; i++) {
        //cout << "Rand i = " << i << endl;
        int Ni = iRand(-1000, 1000);
        int Di = iRand(-100, 100);

        cout << "Ni = " << Ni << endl;
        cout << "Di = " << Di << endl;

        BitVector N(16, Ni);
        BitVector D(16, Di);

        // cout << "N = " << N << endl;
        // cout << "D = " << D << endl;

        Simulator sim(*f);
        sim.setInput("N", N);
        sim.setInput("D", D);
        sim.evaluate();
        auto quot = sim.getOutput("Q");
        //cout << "Quotient         = " << quot << endl;

        REQUIRE(quot == BitVector(16, Ni / Di));
      }
    }

    SECTION("Synthesizing the newton-raphson divide function") {

      SECTION("Combinational synthesis") {
        SynthesisConstraints constraints;
        constraints.setMaxCycles(0);

        synthesizeVerilog(f, constraints);

        //REQUIRE(runVerilatorTB(f->getName()));
      }
    }
    
  }
}
