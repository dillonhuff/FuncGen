#pragma once

#include "language.h"

using namespace std;

namespace FuncGen {

  class FixedPoint {
  public:
    BitVector sign;
    BitVector bits;
    int exponent;

    FixedPoint(const BitVector& bits_, const int exp_) :
      sign(BitVector(1, 0)), bits(bits_), exponent(exp_) {}
    
    FixedPoint(const char sign_, const BitVector& bits_, const int exp_) :
      sign(BitVector(1, sign_)), bits(bits_), exponent(exp_) {}

    FixedPoint(const BitVector sign_, const BitVector& bits_, const int exp_) :
      sign(sign_), bits(bits_), exponent(exp_) {}
    
    int bitLength() const {
      return bits.bitLength();
    }
  };

  FixedPoint flipSign(const FixedPoint& b) {
    FixedPoint flipped = b;
    flipped.sign = ~b.sign;
    return flipped;
  }

  static inline
  bool operator==(const FixedPoint& a, const FixedPoint& b) {
    return (a.sign == b.sign) && (a.bits == b.bits) && (a.exponent == b.exponent);
  }

  static inline
  std::ostream& operator<<(std::ostream& out, const FixedPoint& n) {
    for (int i = n.bitLength() - 1; i >= 0; i--) {
      out << n.bits.get(i);
      if (i == -n.exponent) {
        out << ".";
      }
    }

    return out;
  }

  static inline
  FixedPoint multiply(const FixedPoint& a, const FixedPoint& b) {
    auto aBitsExt = zero_extend(2*a.bitLength(), a.bits);
    auto bBitsExt = zero_extend(2*b.bitLength(), b.bits);
    auto prodBits = mul_general_width_bv(aBitsExt, bBitsExt);

    BitVector signBit(1, 0);
    if (a.sign != b.sign) {
      signBit = BitVector(1, 1);
    }

    cout << "Prod bits = " << prodBits << endl;

    assert(a.exponent == b.exponent);
    
    int sumExp = b.exponent;// + a.exponent;
    return {signBit, slice(lshr(prodBits, BitVector(32, -sumExp)), 0, a.bitLength()), sumExp};
  }

  static inline
  FixedPoint mul(const FixedPoint& a, const FixedPoint& b) {
    return multiply(a, b);
  }

  static inline
  quad_value high_bit(const BitVector& b) {
    return b.get(b.bitLength() - 1);
  }

  static inline
  std::pair<BitVector, BitVector>
  twos_complement_to_sign_magnitude(const BitVector twosComp) {
    assert(twosComp.bitLength() > 1);

    if (high_bit(twosComp).binary_value() == 0) {
      return {BitVector(1, high_bit(twosComp).binary_value()),
          slice(twosComp, 0, twosComp.bitLength() - 1)};
    }
    assert(false);
    // BitVector sm(twosComp.size(), 0);
    // for (
  }

  static inline
  BitVector sign_magnitude_to_twos_complement(const BitVector& sign,
                                              const BitVector& magnitude) {
    assert(sign.bitLength() == 1);
    
    BitVector res(magnitude.bitLength() + 1, 0);
    for (int i = 0; i < magnitude.bitLength(); i++) {
      res.set(i, magnitude.get(i));
    }

    if (sign == BitVector(1, 0)) {
      return res;
    }

    res.set(res.bitLength() - 1, 0);
    res = ~res;
    return add_general_width_bv(res, BitVector(res.bitLength(), 1));
  }

  static inline
  FixedPoint add(const FixedPoint& a, const FixedPoint& b) {
    assert(b.exponent == a.exponent);
    assert(b.bits.bitLength() == a.bits.bitLength());

    auto atc = sign_magnitude_to_twos_complement(a.sign, a.bits);
    auto btc = sign_magnitude_to_twos_complement(b.sign, b.bits);
    auto tcRes = add_general_width_bv(atc, btc);

    cout << "tcres = " << tcRes << endl;
    auto smRes = twos_complement_to_sign_magnitude(tcRes);
    return {smRes.first, smRes.second, b.exponent};
  }
  
  static inline
  FixedPoint sub(const FixedPoint& a, const FixedPoint& b) {
    return add(a, flipSign(b));
  }
  
  static inline
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

  static inline
  FixedPoint rightTruncate(int toTruncate, const FixedPoint& num) {
    BitVector truncatedBits(num.bitLength() - toTruncate, 0);
    for (int i = num.bitLength() - 1; i >= toTruncate; i--) {
      truncatedBits.set(i - toTruncate, num.bits.get(i));
    }
    return {truncatedBits, num.exponent + toTruncate};
  }

  static inline
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

  static inline
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

  static inline
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

  static inline
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

  static inline
  double fixedPointToDouble(const FixedPoint& fp) {
    return pow(2, fp.sign.get(0).binary_value())*approximateFixedPoint(fp.bits, -fp.exponent);
  }

}
