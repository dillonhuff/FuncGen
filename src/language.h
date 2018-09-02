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

    virtual ExpressionType type() const = 0;
    // {
    //   return EXPRESSION_TYPE_VARIABLE;
    // }

    Data* getData() const {
      return dataType;
    }

    int bitWidth() const {
      assert(dataType != nullptr);
      
      return dataType->bitWidth();
    }

    virtual ~Expression() {}
  };

  class Variable : public Expression {

    std::string name;
    
  public:

    Variable(Data* dataType_, const std::string& name_) : Expression(dataType_), name(name_) {}

    std::string getName() const {
      return name;
    }

    virtual ExpressionType type() const {
      return EXPRESSION_TYPE_VARIABLE;
    }

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
    STATEMENT_TYPE_PRINT,
    STATEMENT_TYPE_BLOCK,
  };

  class Statement {
  public:

    virtual std::string toString(const int indentLevel) const {
      return tab(indentLevel) + "STATEMENT";
    }

    virtual StatementType type() const = 0;

    virtual ~Statement() {}
  };

  class BlockStatement : Statement {

    std::vector<Statement*> stmts;
    
  public:

    BlockStatement(const std::vector<Statement*>& stmts_) : stmts(stmts_) {}

    std::vector<Statement*> getStatements() const {
      return stmts;
    }

    void addStmt(Statement* stmt) {
      stmts.push_back(stmt);
    }

    ~BlockStatement() {
      for (auto stmt : stmts) {
        delete stmt;
      }
    }

    std::string toString(const int indentLevel) const {
      string str = tab(indentLevel) + "{";
      for (auto stmt : stmts) {
        str += stmt->toString(indentLevel + 1);
      }
      str += tab(indentLevel) + "}";
      return str;
    }

    virtual StatementType type() const {
      return STATEMENT_TYPE_BLOCK;
    }

  };
  
  class PrintStatement : public Statement {
    std::string text;
    std::vector<Expression*> exprs;

  public:

    PrintStatement(const std::string& text_) : text(text_), exprs({}) {}
    PrintStatement(const std::string& text_, const std::vector<Expression*>& exprs_) : text(text_), exprs(exprs_) {}

    std::string toString(const int indentLevel) const {
      return tab(indentLevel) + "\"" + text + "\"";
    }

    std::string getText() const {
      return text;
    }

    std::vector<Expression*> getExprs() const {
      return exprs;
    }
    
    virtual StatementType type() const {
      return STATEMENT_TYPE_PRINT;
    }

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
    //std::vector<Statement*> statements;
    BlockStatement* stmt;
    std::map<std::string, Value*> inputs;
    std::map<std::string, Value*> outputs;

    std::map<std::string, Value*> values;
    
  public:

    Function(Context& context_,
             const std::string& name_,
             const std::map<std::string, Data*>& inputs_,
             const std::map<std::string, Data*>& outputs_) :
      uniqueNum(1), context(context_), name(name_), stmt(new BlockStatement({})) {

      for (auto in : inputs_) {
        Value* v = new Variable(in.second, in.first);
        inputs.insert({in.first, v});
        values.insert({in.first, v});
      }

      for (auto out : outputs_) {
        Value* v = new Variable(out.second, out.first);
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
      return stmt->getStatements();
    }

    Statement* assignStmt(Value* res, Expression* expr) {
      return new Assignment(res, expr);
    }

    Value* caseStatement(Value* in, Expression* trueExpr, Expression* falseExpr);
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
    Value* makeUniqueValue(const std::string& name, const int width);

    inline Value* freshVal(const int width) {
      return makeUniqueValue(width);
    }

    inline Value* freshVal(const std::string& name, const int width) {
      return makeUniqueValue(name, width);
    }
    
    Value* unsignedDivide(Value* a, Value* b);
    Value* multiply(Value* a, Value* b);
    Value* subtract(Value* a, Value* b);
    Value* add(Value* a, Value* b);
    Value* zeroExtend(const int resWidth, Value* v);
    Value* shiftLeft(const int shiftValue, Value* v);
    Value* logicalShiftRight(const int shiftValue, Value* v);
    Value* invert(Value* v);
    Value* leadZeroCount(Value* v);
    Value* shiftLeftVariable(Value* v, Value* shiftAmount);
    Value* logicalShiftRightVariable(Value* v, Value* shiftAmount);
    Value* equals(Value* a, Value* b);
    void printStmt(const std::string& str);
    void printStmt(const std::string& str, const std::vector<Expression*>& exprs);

    Function* getBuiltinSlice(const int inWidth,
                              const int end,
                              const int start);

    Function* getBuiltin(const std::string& name, const int width);

    void assign(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      cout << "a/b not null" << endl;
      
      if (!sameWidth(*a, *b)) {
        std::cout << "ERROR: Mismatched widths in assign " << a->bitWidth() << " and " << b->bitWidth() << std::endl;
        assert(false);
      }

      cout << "Assigning" << endl;

      stmt->addStmt(new Assignment(a, b));
    }
    
    Value* slice(const int end, const int start, Value* v); // {
    //   std::string shiftName = "slice_" + std::to_string(end) + "_" + std::to_string(start);

    //   Value* freshValue = makeUniqueValue(end - start + 1);
    //   stmt.addStmt(new Assignment(freshValue, new FunctionCall(shiftName, {{"in", v}})));

    //   return freshValue;
    // }

    Value* equalsExpr(Value* a, Value* b);    

    Value* sliceExpr(const int end, const int start, Value* v) {
      return new FunctionCall(getBuiltinSlice(v->bitWidth(), end, start), {{"in", v}});
    }
    
    Expression* fpMul(Value* a, Value* b, const int decimalPlace) {
      assert(a->bitWidth() == b->bitWidth());

      Expression* aBitsExt = zextExpr(a, 2*a->bitWidth());
      Expression* bBitsExt = zextExpr(b, 2*b->bitWidth());

      Expression* prodBits = timesExpr(aBitsExt, bBitsExt);
      Expression* res = sliceExpr(a->bitWidth(), 0, lshrExpr(prodBits, decimalPlace));

      return res;
    }

    Expression* plusExpr(Value* a, Value* b) {
      return new FunctionCall(getBuiltin("add",a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }

    Expression* subExpr(Value* a, Value* b) {
      return new FunctionCall(getBuiltin("subtract", a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }
    
    Expression* timesExpr(Value* a, Value* b) {
      return new FunctionCall(getBuiltin("multiply", a->bitWidth()),
                              {{"in0", a}, {"in1", b}});
    }

    Expression* zextExpr(Value* a, int width) {
      return new FunctionCall(getBuiltin("zero_extend_" + std::to_string(width), width),
                              {{"in", a}});
    }

    Expression* lshrExpr(Value* a, int width) {
      return new FunctionCall(getBuiltin("logical_shift_right_" + std::to_string(width), a->bitWidth()),
                              {{"in", a}});
    }
    
    Value* constant(const int bitWidth, const int value);
    Value* constant(const BitVector& b);

    void repeat(const int numIterations, Statement* e) {
      stmt->addStmt(new RepeatStmt(numIterations, e));
    }

    void setReturn(const std::string& returnName, const Value* value) {
      stmt->addStmt(new ReturnStmt());
    }

    ~Function() {
      delete stmt;
      // for (auto stmt : statements) {
      //   delete stmt;
      // }

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

    Function* getBuiltin(const std::string& name, const int width);

    Function* getBuiltinSlice(const int inWidth,
                              const int end,
                              const int start);
    
    Function* getFunction(const std::string& name) const {
      return map_find(name, functions);
    }

    bool hasFunction(const std::string& name) const {
      return contains_key(name, functions);
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

}

