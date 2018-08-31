#pragma once

#include "algorithm.h"
#include "quad_value_bit_vector.h"

#include <fstream>

using namespace dbhc;
using namespace bsim;
using namespace std;

namespace FuncGen {

  class Context;
  class Function;

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


  enum ExpressionType {
    EXPRESSION_TYPE_FUNCTION_CALL,
    EXPRESSION_TYPE_CONSTANT,
    EXPRESSION_TYPE_VARIABLE,
    EXPRESSION_TYPE_CASE,
  };

  class Expression {

    Data* dataType;

  public:

    Expression(Data* dataType_) : dataType(dataType_) {}

    virtual ExpressionType type() const {
      return EXPRESSION_TYPE_VARIABLE;
    }

    Data* getData() const {
      return dataType;
    }

    int bitWidth() const {
      assert(dataType != nullptr);
      
      return dataType->bitWidth();
    }

    virtual ~Expression() {}
  };

  typedef Expression Value;

  class ConstantValue : public Value {
    BitVector val;

  public:
    ConstantValue(Data* dataType_, const BitVector& val_) : Value(dataType_), val(val_) {}

    virtual ExpressionType type() const {
      return EXPRESSION_TYPE_CONSTANT;
    }

    BitVector getValue() const {
      return val;
    }
    
  };

  static inline bool sameWidth(const Value& a, const Value& b) {
    cout << "Getting same width" << endl;
    return a.bitWidth() == b.bitWidth();
  }

  static inline bool sameWidth(const Value* a, const Value* b) {
    return sameWidth(*a, *b);
  }

  typedef std::vector<std::pair<BitVector, Expression*> > Cases;

  enum StatementType {
    STATEMENT_TYPE_FUNCTION_CALL,
    STATEMENT_TYPE_CASE,
    STATEMENT_TYPE_OPERATOR,
    STATEMENT_TYPE_RETURN,
    STATEMENT_TYPE_ASSIGNMENT,
    STATEMENT_TYPE_REPEAT,
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
    Value* result;
    Value* in;
    std::vector<std::pair<BitVector, Expression*> > cases;

  public:

    // Case(Value* result_,
    //      Value* in_,
    //      const std::vector<std::pair<BitVector, BitVector> >& cases_) :
    //   result(result_), in(in_), cases() {}

    Case(Value* result_,
         Value* in_,
         const std::vector<std::pair<BitVector, Expression*> >& cases_) :
      result(result_), in(in_), cases(cases_) {}
    
    Value* getResult() const {
      return result;
    }

    Value* getInput() const {
      return in;
    }
    
    std::vector<std::pair<BitVector, Expression*> >& getCases() {
      return cases;
    }

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

  //class FunctionCall : public Statement {
  class FunctionCall : public Expression {
    Function* function;
    std::map<std::string, Value*> inputs;
    
  public:

    FunctionCall(Function* function,
                 const std::map<std::string, Value*>& inputs_);

    const std::map<std::string, Value*>& getInputs() const {
      return inputs;
    }

    Value* getInput(const std::string& name) const {
      assert(contains_key(name, inputs));

      return map_find(name, inputs);
    }

    std::string functionName() const;

    virtual ExpressionType type() const {
      return EXPRESSION_TYPE_FUNCTION_CALL;
    }
  };

  class ReturnStmt : public Statement {
  public:
    virtual StatementType type() const {
      return STATEMENT_TYPE_RETURN;
    }
  };

  class RepeatStmt : public Statement {
    int numIters;
    Statement* stmt;

  public:
    RepeatStmt(int numIterations_, Statement* stmt_) :
      numIters(numIterations_), stmt(stmt_) {}

    Statement* body() const { return stmt; }

    int numIterations() const { return numIters; }

    virtual StatementType type() const {
      return STATEMENT_TYPE_REPEAT;
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

    Statement* assignStmt(Value* res, Expression* expr) {
      return new Assignment(res, expr);
    }

    Value* caseStatement(Value* in, const std::vector<std::pair<BitVector, BitVector> >& cases);

    Value* caseStatement(Value* in, const Cases& cases);

    Expression* unop(const std::string& name, Value* arg);
    Expression* unop(Function* const f, Value* arg);
    
    Value* functionCall(const std::string& str, const std::map<std::string, Value*>& args);
    Value* functionCall(const std::string& str, Value* arg);

    Value* getValue(const std::string& name) {
      return map_find(name, values);
    }

    Value* makeUniqueValue(const int width);

    inline Value* freshVal(const int width) {
      return makeUniqueValue(width);
    }

    Value* unsignedDivide(Value* a, Value* b);// {
    //   assert(a != nullptr);
    //   assert(b != nullptr);

    //   assert(sameWidth(*a, *b));

    //   std::string divideName = "unsigned_divide_" + std::to_string(a->bitWidth());
    //   Value* freshValue = makeUniqueValue(a->bitWidth());
    //   statements.push_back(new Assignment(freshValue,
    //                                       new FunctionCall(getContext().getBuiltin("unsigned_divide", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    //   return freshValue;
    // }

    Value* multiply(Value* a, Value* b); // {
    //   assert(a != nullptr);
    //   assert(b != nullptr);

    //   assert(sameWidth(*a, *b));

    //   std::string divideName = "multiply_" + std::to_string(a->bitWidth());
    //   Value* freshValue = makeUniqueValue(a->bitWidth());
    //   statements.push_back(new Assignment(freshValue, new FunctionCall(divideName, {{"in0", a}, {"in1", b}})));
    //   return freshValue;
    // }

    Value* subtract(Value* a, Value* b); // {
    //   assert(a != nullptr);
    //   assert(b != nullptr);

    //   assert(sameWidth(*a, *b));

    //   std::string divideName = "subtract_" + std::to_string(a->bitWidth());
    //   Value* freshValue = makeUniqueValue(a->bitWidth());
    //   statements.push_back(new Assignment(freshValue, new FunctionCall(divideName, {{"in0", a}, {"in1", b}})));
    //   return freshValue;
    // }

    Value* add(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      assert(sameWidth(*a, *b));

      std::string divideName = "add_" + std::to_string(a->bitWidth());
      Value* freshValue = makeUniqueValue(a->bitWidth());
      statements.push_back(new Assignment(freshValue, new FunctionCall(divideName, {{"in0", a}, {"in1", b}})));
      return freshValue;
    }
    
    Value* zeroExtend(const int resWidth, Value* v) {
      std::string zextName = "zero_extend_" + std::to_string(resWidth);

      Value* freshValue = makeUniqueValue(resWidth);
      statements.push_back(new Assignment(freshValue, new FunctionCall(zextName, {{"in", v}})));
      return freshValue;
    }

    Value* shiftLeft(const int shiftValue, Value* v) {
      std::string shiftName = "shift_left_" + std::to_string(shiftValue);

      Value* freshValue = makeUniqueValue(v->bitWidth());
      statements.push_back(new Assignment(freshValue, new FunctionCall(shiftName, {{"in", v}})));

      return freshValue;
    }

    Value* logicalShiftRight(const int shiftValue, Value* v) {
      std::string shiftName = "logical_shift_right_" + std::to_string(shiftValue);

      Value* freshValue = makeUniqueValue(v->bitWidth());
      statements.push_back(new Assignment(freshValue, new FunctionCall(shiftName, {{"in", v}})));

      return freshValue;
    }

    Value* invert(Value* v) {
      std::string invName = "invert_" + std::to_string(v->bitWidth());

      Value* freshValue = makeUniqueValue(v->bitWidth());
      statements.push_back(new Assignment(freshValue, new FunctionCall(invName, {{"in", v}})));

      return freshValue;
    }
    
    void assign(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      cout << "a/b not null" << endl;
      
      if (!sameWidth(*a, *b)) {
        std::cout << "ERROR: Mismatched widths in assign " << a->bitWidth() << " and " << b->bitWidth() << std::endl;
        assert(false);
      }

      cout << "Assigning" << endl;

      statements.push_back(new Assignment(a, b));
    }
    
    Value* slice(const int end, const int start, Value* v) {
      std::string shiftName = "slice_" + std::to_string(end) + "_" + std::to_string(start);

      Value* freshValue = makeUniqueValue(end - start + 1);
      statements.push_back(new Assignment(freshValue, new FunctionCall(shiftName, {{"in", v}})));

      return freshValue;
    }

    Expression* plusExpr(Value* a, Value* b) {
      return new FunctionCall("add_" + std::to_string(a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }

    Expression* subExpr(Value* a, Value* b) {
      return new FunctionCall("subtract_" + std::to_string(a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }
    
    Expression* timesExpr(Value* a, Value* b) {
      return new FunctionCall("multiply_" + std::to_string(a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }
    
    Value* addEquals(const Value* a, const Value* b) {
      return nullptr;
    }

    Value* constant(const int bitWidth, const int value);
    Value* constant(const BitVector& b);

    void repeat(const int numIterations, Statement* e) {
      statements.push_back(new RepeatStmt(numIterations, e));
    }

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

    Function* getBuiltin(const std::string& name, const int width) const {
      assert(false);
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

    Function* newUnaryFunction(const std::string& name,
                               const std::string& arg0Name,
                               Data* arg0Type,
                               const std::string& arg1Name,
                               Data* arg1Type) {
      return newFunction(name, {{arg0Name, arg0Type}}, {{arg1Name, arg1Type}});
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

  // // Problem: How do I write statements and functions in a way that is easy
  // // but also embedded in C++? Expressions are doable. Why? I guess because
  // // they can be constructed without reference to the current enclosing statement?
  // // Whereas statements are usually constructed recursively inside one another.
  // // This makes things a little trickier.
  // static Context* globalContext;

  // void setGlobalContext(Context* c);

}

