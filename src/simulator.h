#pragma once

#include "language.h"
#include "fixed_point.h"

using namespace std;

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
      cout << "Setting input " << inputName << endl;
      variableValues[map_find(inputName, variableNames)] = value;
      cout << "Done" << endl;
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

        std::string ss = "zero_extend_";
        int width = stoi(name.substr(ss.size()));
        return zero_extend(width, evaluateExpression(a));

      } else if (isUnsignedDivide(name)) {
        Value* toDivide = call.getInput("in0");
        Value* divisor = call.getInput("in1");

        return unsigned_divide(evaluateExpression(toDivide), evaluateExpression(divisor));
      } else if (hasPrefix(name, "logical_shift_right_variable")) {

        Value* toShift = call.getInput("in0");
        Value* shiftAmount = call.getInput("in1");

        return lshr(evaluateExpression(toShift), evaluateExpression(shiftAmount));

      } else if (hasPrefix(name, "shift_left_variable")) {
        cout << "Shift left variable" << endl;
        Value* toShift = call.getInput("in0");
        Value* shiftAmount = call.getInput("in1");
        return shl(evaluateExpression(toShift), evaluateExpression(shiftAmount));
        
      } else if (isShiftLeft(name)) {
        std::string pre = "shift_left_";
        int shiftAmount = stoi(name.substr(pre.size()));

        Value* toShift = call.getInput("in");


        return shl(evaluateExpression(toShift), BitVector(32, shiftAmount));

      } else if (hasPrefix(name, "equals")) {

        Value* a = call.getInput("in0");
        Value* b = call.getInput("in1");

        return BitVector(1, evaluateExpression(a) == evaluateExpression(b));
        
      } else if (hasPrefix(name, "multiply_")) {
        Value* toDivide = call.getInput("in0");
        Value* divisor = call.getInput("in1");

        //Value* res = call.getResult();

        return mul_general_width_bv(evaluateExpression(toDivide), evaluateExpression(divisor));
        //setValue(res, mul_general_width_bv(getValue(toDivide), getValue(divisor)));
        
      } else if (hasPrefix(name, "slice_")) {
        std::string pre = "slice_";

        //std::cout << "slice name       = " << name << std::endl;
        size_t sz;
        int endSlice = stoi(name.substr(pre.size()), &sz);

        //cout << "endSlice = " << endSlice << endl;
        //cout << "next substr = " << name.substr(sz + 1) << endl;
        int startSlice = stoi(name.substr(pre.size() + sz + 1)); //stoi(name.substr(firstNumEndIndex)); //pre.size() + 2));

        //std::cout << "Start slice at " << startSlice << " end at " << endSlice << std::endl;
        Value* toShift = call.getInput("in");


        auto sliceRes = slice(evaluateExpression(toShift), startSlice, endSlice + 1);

        return sliceRes;

      } else if (hasPrefix(name, "invert_")) {

        Value* toInvert = call.getInput("in");
        return ~evaluateExpression(toInvert);

      } else if (hasPrefix(name, "logical_shift_right_")) {

        std::string pre = "logical_shift_right_";
        int shiftAmount = stoi(name.substr(pre.size()));

        Value* toShift = call.getInput("in");
        return lshr(evaluateExpression(toShift), BitVector(32, shiftAmount));

      } else if (hasPrefix(name, "add_")) {

        Value* in0 = call.getInput("in0");
        Value* in1 = call.getInput("in1");

        return add_general_width_bv(evaluateExpression(in0), evaluateExpression(in1));

      } else if (hasPrefix(name, "subtract_")) {
        Value* in0 = call.getInput("in0");
        Value* in1 = call.getInput("in1");

        return sub_general_width_bv(evaluateExpression(in0), evaluateExpression(in1));
      } else if (hasPrefix(name, "count_leading_zeros")) {
        Value* in = call.getInput("in");

        return BitVector(in->bitWidth(), num_leading_zeros(evaluateExpression(in)));
      } else {
        Function* toCall = f.getContext().getFunction(name);

        cout << "Calling " << toCall->getName() << endl;
        assert(toCall->getStatements().size() > 0);

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
      assert(expr != nullptr);
      
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
            setValue(assign.getResult(), evaluateExpression(c.second));
            return;
          }
        }

        assert(false);

      } else if (stmt.type() == STATEMENT_TYPE_REPEAT) {
        const RepeatStmt& assign = static_cast<const RepeatStmt&>(stmt);
        for (int i = 0; i < assign.numIterations(); i++) {
          evaluateStatement(*(assign.body()));
        }
      } else if (stmt.type() == STATEMENT_TYPE_PRINT) {
        const PrintStatement& printStmt = static_cast<const PrintStatement&>(stmt);

        std::string textStr = "";
        int i = 0;
        int exprInd = 0;
        auto text = printStmt.getText();
        while(i < (int) text.size()) {
          char nextChar = text[i];
          if (nextChar != '%') {
            cout << nextChar;
            i++;
          } else {
            assert(text[i + 1] == 'b'); // Only supported format specifier
            BitVector value = evaluateExpression(printStmt.getExprs()[exprInd]);
            cout << value;
            exprInd++;
            i += 2;
          }
        }

        cout << endl;

        //cout << textStr << endl;
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
