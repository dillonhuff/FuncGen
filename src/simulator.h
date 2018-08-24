#pragma once

#include "language.h"

namespace FuncGen {


  static inline
  bool hasPrefix(const std::string& name, const std::string& prefix) {
    return name.substr(0, prefix.size()) == prefix;
  }

  static inline bool isZext(const std::string& name) {
    return hasPrefix(name, "zero_extend_");
  }

  static inline bool isUnsignedDivide(const std::string& name) {
    return hasPrefix(name, "unsigned_divide_");
  }

  static inline bool isShiftLeft(const std::string& name) {
    return hasPrefix(name, "shift_left_");
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

    void printValueStore() const {
      std::cout << "Values" << std::endl;
      for (auto name : variableNames) {
        std::cout << "\t" << name.first << " = " << map_find(name.second, variableValues) << std::endl;
      }
    }

    BitVector getOutput(const std::string& outputName) const {
      return map_find(map_find(outputName, variableNames), variableValues);
    }

    BitVector getValue(Value* v) const {
      if (v->type() == EXPRESSION_TYPE_VARIABLE) {
        assert(contains_key(v, variableValues));
      
        return map_find(v, variableValues);
      } else {
        assert(v->type() == EXPRESSION_TYPE_CONSTANT);
        return static_cast<ConstantValue*>(v)->getValue();
      }
    }

    void setInput(const std::string& inputName, const BitVector& value) {
      variableValues[map_find(inputName, variableNames)] = value;
    }

    void setValue(Value* v, const BitVector& value) {
      if (!(v->bitWidth() == value.bitLength())) {
        std::cout << "ERROR: Bit Widths do not match in setValue, " << v->bitWidth() << " and " << value.bitLength() << std::endl;
        assert(false);
      }
      
      variableValues[v] = value;
    }

    BitVector evaluateFunctionCall(const FunctionCall& call) {
      //assert(false);

      std::string name = call.functionName();
      if (isZext(name)) {
        Value* a = call.getInput("in");
        //Value* res = call.getResult();

        std::string ss = "zero_extend_";
        int width = stoi(name.substr(ss.size()));
        return zero_extend(width, evaluateExpression(a));

        //setValue(res, zero_extend(res->bitWidth(), getValue(a)));
      } else if (isUnsignedDivide(name)) {
        Value* toDivide = call.getInput("in0");
        Value* divisor = call.getInput("in1");

        //Value* res = call.getResult();

        return unsigned_divide(evaluateExpression(toDivide), evaluateExpression(divisor));
        //setValue(res, unsigned_divide(getValue(toDivide), getValue(divisor)));
      } else if (isShiftLeft(name)) {
        std::string pre = "shift_left_";
        int shiftAmount = stoi(name.substr(pre.size()));

        Value* toShift = call.getInput("in");

        //Value* res = call.getResult();

        return shl(evaluateExpression(toShift), BitVector(32, shiftAmount));
        //setValue(res, shl(getValue(toShift), BitVector(32, shiftAmount)));
      } else if (hasPrefix(name, "multiply_")) {
        Value* toDivide = call.getInput("in0");
        Value* divisor = call.getInput("in1");

        //Value* res = call.getResult();

        return mul_general_width_bv(evaluateExpression(toDivide), evaluateExpression(divisor));
        //setValue(res, mul_general_width_bv(getValue(toDivide), getValue(divisor)));
        
      } else if (hasPrefix(name, "slice_")) {
        std::string pre = "slice_";
        int endSlice = stoi(name.substr(pre.size()));
        int startSlice = stoi(name.substr(pre.size() + 2));

        //std::cout << "Start slice at " << startSlice << " end at " << endSlice << std::endl;
        Value* toShift = call.getInput("in");

        //Value* res = call.getResult();

        auto sliceRes = slice(evaluateExpression(toShift), startSlice, endSlice + 1);
        //std::cout << "SliceRes = " << sliceRes << std::endl;
        return sliceRes;
        //setValue(res, sliceRes);
      } else if (hasPrefix(name, "logical_shift_right_")) {
        std::string pre = "logical_shift_right_";
        int shiftAmount = stoi(name.substr(pre.size()));

        Value* toShift = call.getInput("in");

        //Value* res = call.getResult();

        return lshr(evaluateExpression(toShift), BitVector(32, shiftAmount));
      //setValue(res, lshr(getValue(toShift), BitVector(32, shiftAmount)));
      } else if (hasPrefix(name, "add_")) {
        Value* in0 = call.getInput("in0");
        Value* in1 = call.getInput("in1");

        return add_general_width_bv(evaluateExpression(in0), evaluateExpression(in1));

      } else if (hasPrefix(name, "subtract_")) {
        Value* in0 = call.getInput("in0");
        Value* in1 = call.getInput("in1");

        return sub_general_width_bv(evaluateExpression(in0), evaluateExpression(in1));
      } else {
        Function* toCall = f.getContext().getFunction(name);

        Simulator s(*toCall);
        for (auto input : call.getInputs()) {
          s.setInput(input.first, evaluateExpression(input.second));
        }
        s.evaluate();

        std::cout << "Output result = " << s.getOutput(toCall->outputValueName()) << std::endl;

        //setValue(call.getResult(), s.getOutput(toCall->outputValueName()));

        return s.getOutput(toCall->outputValueName());
        //assert(false);
      }
    }

    BitVector evaluateExpression(Expression* expr) {
      if (expr->type() == EXPRESSION_TYPE_FUNCTION_CALL) {
        return evaluateFunctionCall(*static_cast<FunctionCall*>(expr));
      } else if (expr->type() == EXPRESSION_TYPE_CONSTANT) {
        return static_cast<ConstantValue*>(expr)->getValue();
      } else if (expr->type() == EXPRESSION_TYPE_VARIABLE) {
        return getValue(static_cast<Value*>(expr));
      }
      assert(false);
    }

    void evaluateStatement(Statement& stmt) {

      if (stmt.type() == STATEMENT_TYPE_CASE) {
        Case& assign = static_cast<Case&>(stmt);

        Value* input = assign.getInput();
        BitVector inVal = getValue(input);
        for (auto c : assign.getCases()) {
          if (inVal == c.first) {
            setValue(assign.getResult(), c.second);
            return;
          }
        }

        assert(false);

      } else if (stmt.type() == STATEMENT_TYPE_REPEAT) {
        const RepeatStmt& assign = static_cast<const RepeatStmt&>(stmt);
        for (int i = 0; i < assign.numIterations(); i++) {
          evaluateStatement(*(assign.body()));
        }
      } else {
        assert(stmt.type() == STATEMENT_TYPE_ASSIGNMENT);
        const Assignment& assign = static_cast<const Assignment&>(stmt);
        Value* a = assign.getLHS();
        //std::cout << "RHS of assignment = " << getValue(assign.getRHS()) << std::endl;
        //std::cout << "LHS of assignment = " << assign.getLHS() << std::endl;
        setValue(a, evaluateExpression(assign.getRHS()));
      }
    }

    void evaluate() {
      for (auto stmt : f.getStatements()) {
        evaluateStatement(*stmt);
      }
    }
  };
}
