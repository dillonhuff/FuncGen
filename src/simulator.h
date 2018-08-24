#pragma once

#include "language.h"

namespace FuncGen {

  static inline
  bsim::quad_value_bit_vector unsigned_divide(const BitVector& a,
                                              const BitVector& b) {
    BitVector quotient(a.bitLength(), 0);

    return quotient;
  }

  bool hasPrefix(const std::string& name, const std::string& prefix) {
    return name.substr(0, prefix.size()) == prefix;
  }

  static inline bool isZext(const std::string& name) {
    return hasPrefix(name, "zero_extend_");
    //return name.substr(0, 12) == "zero_extend_";
  }

  static inline bool isUnsignedDivide(const std::string& name) {
    return hasPrefix(name, "unsigned_divide_");

    // std::cout << "Name = " << name << std::endl;
    // std::cout << "sub  = " << name.substr(0, 16) << std::endl;
    // return name.substr(0, 16) == "unsigned_divide_";
  }

  static inline bool isShiftLeft(const std::string& name) {
    return hasPrefix(name, "shift_left_");
    // std::cout << "Name = " << name << std::endl;
    // std::cout << "sub  = " << name.substr(0, 16) << std::endl;
    // return name.substr(0, 16) == "shift_left_";
  }
  
  class Simulator {

    Function& f;
    std::map<std::string, Value*> variableNames;
    std::map<Value*, BitVector> variableValues;
    
  public:
    Simulator(Function& f_) : f(f_) {
      // Data vs. Value? What is the difference?

      for (auto& v : f.outputMap()) {
        variableNames.insert({v.first, v.second});
        variableValues.insert({v.second, unknown_bv(v.second->bitWidth())});
      }

      for (auto& v : f.inputMap()) {
        variableNames.insert({v.first, v.second});
        variableValues.insert({v.second, unknown_bv(v.second->bitWidth())});
      }

    }

    BitVector getOutput(const std::string& outputName) const {
      return map_find(map_find(outputName, variableNames), variableValues);
    }

    BitVector getValue(Value* v) const {
      assert(contains_key(v, variableValues));
      
      return map_find(v, variableValues);
    }

    void setInput(const std::string& inputName, const BitVector& value) {
      variableValues[map_find(inputName, variableNames)] = value;
    }

    void setValue(Value* v, const BitVector& value) {
      variableValues[v] = value;
    }

    void evaluateFunctionCall(const FunctionCall& call) {
      std::string name = call.functionName();
      if (isZext(name)) {
        Value* a = call.getInput("in");
        Value* res = call.getResult();
        setValue(res, zero_extend(res->bitWidth(), getValue(a)));
      } else if (isUnsignedDivide(name)) {
        Value* toDivide = call.getInput("in0");
        Value* divisor = call.getInput("in1");

        Value* res = call.getResult();

        setValue(res, unsigned_divide(getValue(toDivide), getValue(divisor)));
      } else if (isShiftLeft(name)) {
        std::string pre = "shift_left_";
        int shiftAmount = stoi(name.substr(pre.size()));

        Value* toShift = call.getInput("in");

        Value* res = call.getResult();

        setValue(res, shl(shiftAmount, getValue(toShift)));
      } else {
        std::cout << "ERROR: Unsupported function name " << name << std::endl;
        assert(false);
      }
    }

    void evaluateStatement(const Statement& stmt) {
      if (stmt.type() == STATEMENT_TYPE_FUNCTION_CALL) {
        const FunctionCall& call = static_cast<const FunctionCall&>(stmt);
        evaluateFunctionCall(call);
      } else {
        assert(stmt.type() == STATEMENT_TYPE_ASSIGNMENT);
        const Assignment& assign = static_cast<const Assignment&>(stmt);
        Value* a = assign.getLHS();
        std::cout << "RHS of assignment = " << getValue(assign.getRHS()) << std::endl;
        setValue(a, getValue(assign.getRHS()));
      }
    }

    void evaluate() {
      for (auto stmt : f.getStatements()) {
        evaluateStatement(*stmt);
      }
    }
  };
}
