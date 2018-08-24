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
  
  class Value {

    Data* dataType;

  public:

    Value(Data* dataType_) : dataType(dataType_) {}

    int bitWidth() const {
      return dataType->bitWidth();
    }
  };

  static inline bool sameWidth(const Value& a, const Value& b) {
    return a.bitWidth() == b.bitWidth();
  }

  enum StatementType {
    STATEMENT_TYPE_FUNCTION_CALL,
    STATEMENT_TYPE_CASE,
    STATEMENT_TYPE_OPERATOR,
    STATEMENT_TYPE_RETURN,
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

  class FunctionCall : public Statement {
    std::string funcName;
    Value* result;

    //std::map<std::string, Value*> inputs;
    
  public:
    FunctionCall(const std::string& funcName_,
                 Value* result_,
                 const std::map<std::string, Value*>& inputs_) :
      funcName(funcName_), result(result_) {}

    Value* getResult() const {
      return result;
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
    Context& context;

    std::string name;
    std::vector<Statement*> statements;
    std::map<std::string, Data*> inputs;
    std::map<std::string, Data*> outputs;

    std::set<Value*> values;
    
  public:

    Function(Context& context_,
             const std::string& name_,
             const std::map<std::string, Data*>& inputs_,
             const std::map<std::string, Data*>& outputs_) : context(context_), name(name_), inputs(inputs_), outputs(outputs_) {}

    std::string getName() const {
      return name;
    }

    const std::map<std::string, Data*>& inputMap() const {
      return inputs;
    }

    const std::map<std::string, Data*>& outputMap() const {
      return outputs;
    }

    std::vector<Statement*> getStatements() const {
      return statements;
    }

    Value* addFunctionCall(const std::string& str);

    Value* getValue(const std::string& name) {
      return nullptr;
    }

    Value* makeUniqueValue(const int width); // {
    //   Value* v = new Value(context.arrayType(width));
    //   values.insert(v);
    //   return v;
    // }

    Value* unsignedDivide(Value* a, Value* b) {
      assert(sameWidth(*a, *b));

      std::string divideName = "unsigned_divide_" + std::to_string(a->bitWidth());
      Value* freshValue = makeUniqueValue(a->bitWidth());
      statements.push_back(new FunctionCall(divideName, freshValue, {{"in0", a}, {"in1", b}}));
      return freshValue;
    }

    Value* zeroExtend(const int resWidth, Value* v) {
      std::string divideName = "zero_extend_" + std::to_string(resWidth);

      Value* freshValue = makeUniqueValue(resWidth);
      statements.push_back(new FunctionCall(divideName, freshValue, {{"in", v}}));
      return freshValue;
    }

    Value* shiftLeft(const int shiftValue, Value const * const v) {
      //statements.push_back(new FunctionCall());
      return nullptr;
    }
    
    Value* addSlice(const int end, const int start, Value* v) {
      //statements.push_back(new FunctionCall());
      return nullptr;
    }

    Value* addEquals(const Value* a, const Value* b) {
      //statements.push_back(new FunctionCall());
      return nullptr;
    }

    Value* addCase(const Value* res, const Value* ifTrue, const Value* ifFalse) {
      statements.push_back(new Case());
      return nullptr;
    }

    Value* addConstant(const int bitWidth, const int Value) {
      return nullptr;
    }

    void setReturn(const std::string& returnName, const Value* value) {
      statements.push_back(new ReturnStmt());
    }

    ~Function() {
      for (auto stmt : statements) {
        delete stmt;
      }

      for (auto value : values) {
        delete value;
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

}
