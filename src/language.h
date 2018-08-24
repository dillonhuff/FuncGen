#pragma once

#include "algorithm.h"
#include "quad_value_bit_vector.h"

#include <fstream>

using namespace dbhc;
using namespace bsim;

namespace FuncGen {

  class Context;

  typedef bsim::quad_value_bit_vector BitVector;

  static inline
  std::string tab(const int indentLevel) {
    std::string ind = "";
    for (int i = 0; i < indentLevel; i++) {
      ind += "\t";
    }
    return ind;
  }

  enum DataType {
    DATA_TYPE_ARRAY,
    DATA_TYPE_CUSTOM
  };

  class Data {
    int bitLength;

  public:

    Data(const int bitLength_) : bitLength(bitLength_) {}

    int bitWidth() const {
      return bitLength;
    }
  };

  enum ValueType {
    VALUE_TYPE_VAR,
    VALUE_TYPE_CONST,
  };

  class Value {

    Data* dataType;

  public:

    Value(Data* dataType_) : dataType(dataType_) {}

    virtual ValueType type() const {
      return VALUE_TYPE_VAR;
    }

    int bitWidth() const {
      return dataType->bitWidth();
    }

    virtual ~Value() {}
  };

  class ConstantValue : public Value {
    BitVector val;

  public:
    ConstantValue(Data* dataType_, const BitVector& val_) : Value(dataType_), val(val_) {}

    virtual ValueType type() const {
      return VALUE_TYPE_CONST;
    }

    BitVector getValue() const {
      return val;
    }
    
  };

  static inline bool sameWidth(const Value& a, const Value& b) {
    return a.bitWidth() == b.bitWidth();
  }

  static inline bool sameWidth(const Value* a, const Value* b) {
    return sameWidth(*a, *b);
  }

  class Table {
  public:
    
  };
  
  enum StatementType {
    STATEMENT_TYPE_FUNCTION_CALL,
    STATEMENT_TYPE_CASE,
    STATEMENT_TYPE_OPERATOR,
    STATEMENT_TYPE_RETURN,
    STATEMENT_TYPE_ASSIGNMENT,
  };

  class Statement {
  public:

    std::string toString(const int indentLevel) const {
      return tab(indentLevel) + "STATEMENT";
    }

    virtual StatementType type() const = 0;

    virtual ~Statement() {}
  };

  class Case : public Statement {
  public:
    virtual StatementType type() const {
      return STATEMENT_TYPE_CASE;
    }
  };

  
  class Operator : public Statement {
  public:
    virtual StatementType type() const {
      return STATEMENT_TYPE_OPERATOR;
    }
  };

  class Assignment : public Statement {
    Value* lhs;
    Value* rhs;

  public:

    Assignment(Value* a, Value* b) : lhs(a), rhs(b) {}

    Value* getLHS() const {
      return lhs;
    }

    Value* getRHS() const {
      return rhs;
    }
    
    virtual StatementType type() const {
      return STATEMENT_TYPE_ASSIGNMENT;
    }
    
  };

  class FunctionCall : public Statement {
    std::string funcName;
    Value* result;

    std::map<std::string, Value*> inputs;
    
  public:
    FunctionCall(const std::string& funcName_,
                 Value* result_,
                 const std::map<std::string, Value*>& inputs_) :
      funcName(funcName_), result(result_), inputs(inputs_) {}

    Value* getResult() const {
      return result;
    }

    const std::map<std::string, Value*>& getInputs() const {
      return inputs;
    }

    Value* getInput(const std::string& name) const {
      assert(contains_key(name, inputs));

      return map_find(name, inputs);
    }

    std::string functionName() const {
      return funcName;
    }

    virtual StatementType type() const {
      return STATEMENT_TYPE_FUNCTION_CALL;
    }
  };

  class ReturnStmt : public Statement {
  public:
    virtual StatementType type() const {
      return STATEMENT_TYPE_RETURN;
    }
  };
  
  class Function {
    int uniqueNum;

    Context& context;

    std::string name;
    std::vector<Statement*> statements;
    std::map<std::string, Value*> inputs;
    std::map<std::string, Value*> outputs;

    std::map<std::string, Value*> values;
    
  public:

    Function(Context& context_,
             const std::string& name_,
             const std::map<std::string, Data*>& inputs_,
             const std::map<std::string, Data*>& outputs_) :
      uniqueNum(1), context(context_), name(name_) {

      for (auto in : inputs_) {
        Value* v = new Value(in.second);
        inputs.insert({in.first, v});
        values.insert({in.first, v});
      }

      for (auto out : outputs_) {
        Value* v = new Value(out.second);
        outputs.insert({out.first, v});
        values.insert({out.first, v});
      }

    }

    Context& getContext() {
      return context;
    }

    std::string getName() const {
      return name;
    }

    std::map<std::string, Value*>& inputMap() {
      return inputs;
    }

    std::map<std::string, Value*>& outputMap() {
      return outputs;
    }

    Value* outputValue() const {
      assert(outputs.size() == 1);
      return (*std::begin(outputs)).second;
    }

    std::string outputValueName() const {
      assert(outputs.size() == 1);
      return (*std::begin(outputs)).first;
    }
    
    std::vector<Statement*> getStatements() const {
      return statements;
    }

    Value* functionCall(const std::string& str, const std::map<std::string, Value*>& args);
    Value* functionCall(const std::string& str, Value* arg);

    Value* getValue(const std::string& name) {
      return map_find(name, values);
    }

    Value* makeUniqueValue(const int width);

    Value* unsignedDivide(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      assert(sameWidth(*a, *b));

      std::string divideName = "unsigned_divide_" + std::to_string(a->bitWidth());
      Value* freshValue = makeUniqueValue(a->bitWidth());
      statements.push_back(new FunctionCall(divideName, freshValue, {{"in0", a}, {"in1", b}}));
      return freshValue;
    }

    Value* multiply(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      assert(sameWidth(*a, *b));

      std::string divideName = "multiply_" + std::to_string(a->bitWidth());
      Value* freshValue = makeUniqueValue(a->bitWidth());
      statements.push_back(new FunctionCall(divideName, freshValue, {{"in0", a}, {"in1", b}}));
      return freshValue;
    }
    
    Value* zeroExtend(const int resWidth, Value* v) {
      std::string zextName = "zero_extend_" + std::to_string(resWidth);

      Value* freshValue = makeUniqueValue(resWidth);
      statements.push_back(new FunctionCall(zextName, freshValue, {{"in", v}}));
      return freshValue;
    }

    Value* shiftLeft(const int shiftValue, Value* v) {
      std::string shiftName = "shift_left_" + std::to_string(shiftValue);

      Value* freshValue = makeUniqueValue(v->bitWidth());
      statements.push_back(new FunctionCall(shiftName, freshValue, {{"in", v}}));

      return freshValue;
    }

    Value* logicalShiftRight(const int shiftValue, Value* v) {
      std::string shiftName = "logical_shift_right_" + std::to_string(shiftValue);

      Value* freshValue = makeUniqueValue(v->bitWidth());
      statements.push_back(new FunctionCall(shiftName, freshValue, {{"in", v}}));

      return freshValue;
    }
    
    void assign(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);
      if (!sameWidth(*a, *b)) {
        std::cout << "ERROR: Mismatched widths in assign " << a->bitWidth() << " and " << b->bitWidth() << std::endl;
        assert(false);
      }

      statements.push_back(new Assignment(a, b));
    }
    
    Value* slice(const int end, const int start, Value* v) {
      std::string shiftName = "slice_" + std::to_string(end) + "_" + std::to_string(start);

      Value* freshValue = makeUniqueValue(end - start + 1);
      statements.push_back(new FunctionCall(shiftName, freshValue, {{"in", v}}));

      return freshValue;
    }

    Value* addEquals(const Value* a, const Value* b) {
      //statements.push_back(new FunctionCall());
      return nullptr;
    }

    Value* addCase(const Value* res, const Value* ifTrue, const Value* ifFalse) {
      statements.push_back(new Case());
      return nullptr;
    }

    Value* addConstant(const int bitWidth, const int value);
    //   ConstantValue* v = new ConstantValue(context.arrayType(bitWidth),
    //                                        BitVector(bitWidth, value));
                                           
    //   values.insert({"const_" + std::to_string(uniqueNum), v});
    //   uniqueNum++;
    //   return v;
    // }

    void setReturn(const std::string& returnName, const Value* value) {
      statements.push_back(new ReturnStmt());
    }

    ~Function() {
      for (auto stmt : statements) {
        delete stmt;
      }

      for (auto value : values) {
        delete value.second;
      }
    }

  };

  std::string listString(const std::vector<std::string>& strs);

  std::vector<std::string> verilogPorts(const Function& f);

  class Context {

    std::map<std::string, Function*> functions;
    std::map<int, Data*> datas;

  public:

    Data* arrayType(const int width) {
      if (!contains_key(width, datas)) {
        datas.insert({width, new Data(width)});

      }
      return map_find(width, datas);
    }

    Function* getFunction(const std::string& name) const {
      return map_find(name, functions);
    }

    Function* newFunction(const std::string& name,
                          const std::map<std::string, Data*>& inputs,
                          const std::map<std::string, Data*>& outputs) {
      auto f = new Function(*this, name, inputs, outputs);
      functions.insert({name, f});
      return f;
    }

    // Synthesize by unfolding the entire compute graph
    void synthesizeNaive(const std::string& functionName) {

      Function* f = map_find(functionName, functions);

      std::string verilogFile = f->getName() + ".v";
      std::ofstream of(verilogFile);

      of << "module " << f->getName() << "(";
      of << listString(verilogPorts(*f));
      of << ");" << std::endl;

      for (auto stmt : f->getStatements()) {
        std::cout << stmt->toString(0) << std::endl;
      }

      of << "endmodule";
        
      of.close();

    }

    ~Context() {
      for (auto f : functions) {
        delete f.second;
      }

      for (auto d : datas) {
        delete d.second;
      }
    }
  };

  Function*
  precomputeTable(const std::string& tableName, Function* f, Context& c);

}
