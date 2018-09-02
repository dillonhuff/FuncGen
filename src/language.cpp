#include "language.h"
#include "simulator.h"

using namespace std;

namespace FuncGen {

  Value*
  Function::caseStatement(Value* in, Expression* trueExpr, Expression* falseExpr) {
    Cases inCases{{BitVector(1, 0), falseExpr}, {BitVector(1, 1), trueExpr}};    
    return caseStatement(in, inCases);
  }
  
  Value*
  Function::caseStatement(Value* in, const std::vector<std::pair<BitVector, BitVector> >& cases) {
    assert(cases.size() > 0);
    int resWidth = cases.at(0).second.bitLength();
    Value* freshValue = makeUniqueValue(resWidth);

    Cases caseExprs;
    for (auto c : cases) {
      caseExprs.push_back({c.first, constant(c.second)});
    }
    stmt->addStmt(new Case(freshValue, in, caseExprs));
    return freshValue;
  }

  Value*
  Function::caseStatement(Value* in, const Cases& cases) {
    assert(cases.size() > 0);

    int resWidth = cases.at(0).second->bitWidth();
    Value* freshValue = makeUniqueValue(resWidth);
    stmt->addStmt(new Case(freshValue, in, cases));

    return freshValue;
  }

  Value* Function::makeUniqueValue(const std::string& name, const int width) {
    Variable* v = new Variable(context.arrayType(width), name);
    uniqueNum++;
    values.insert({name, v});
    return v;

  }

  Value* Function::makeUniqueValue(const int width) {
    string name = "av_" + std::to_string(uniqueNum);
    return makeUniqueValue(name, width);
    // Variable* v = new Variable(context.arrayType(width), name);
    // uniqueNum++;
    // values.insert({name, v});
    // return v;
  }

  Value* Function::constant(const int bitWidth, const int value) {
    return constant(BitVector(bitWidth, value));
    // ConstantValue* v = new ConstantValue(context.arrayType(bitWidth),
    //                                      BitVector(bitWidth, value));
                                           
    // values.insert({"const_" + std::to_string(uniqueNum), v});
    // uniqueNum++;
    // return v;
  }

  Value* Function::constant(const BitVector& b) {
    ConstantValue* v = new ConstantValue(context.arrayType(b.bitLength()), b);
    values.insert({"const_" + std::to_string(uniqueNum), v});
    uniqueNum++;
    return v;

  }

  Value* Function::functionCall(const std::string& str,
                                const std::map<std::string, Value*>& args) {
    Function* f = context.getFunction(str);
    Value* freshValue = makeUniqueValue(f->outputValue()->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(f, args)));
    return freshValue;
  }

  Expression* Function::unop(const std::string& str, Value* arg) {
    Function* f = context.getFunction(str);
    assert(f->inputMap().size() == 1);

    string argName = begin(f->inputMap())->first;
    return new FunctionCall(f, {{argName, arg}});
  }

  Expression* Function::unop(Function* const f, Value* arg) {
    //Function* f = context.getFunction(str);
    assert(f->inputMap().size() == 1);

    string argName = begin(f->inputMap())->first;
    return new FunctionCall(f, {{argName, arg}});
  }
  
  Value* Function::functionCall(const std::string& str, Value* arg) {
    Function* f = context.getFunction(str);
    assert(f->inputMap().size() == 1);

    string argName = begin(f->inputMap())->first;

    Value* freshValue = makeUniqueValue(f->outputValue()->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(f, {{argName, arg}})));
    return freshValue;
    
  }

  Function*
  precomputeTable(const std::string& tableName, Function* f, Context& c) {
    assert(f->inputMap().size() == 1);
    assert(f->outputMap().size() == 1);

    string inputName = (*begin(f->inputMap())).first;
    string outName = (*begin(f->outputMap())).first;
    
    int inWidth = (*begin(f->inputMap())).second->bitWidth();
    int outWidth = (*begin(f->outputMap())).second->bitWidth();

    Function* table = c.newFunction(tableName,
                                    {{"in", c.arrayType(inWidth)}},
                                    {{"out", c.arrayType(outWidth)}});

    Simulator sim(*f);
    vector<pair<BitVector, BitVector> > resultsTable;
    for (int i = 0; i < pow(2, inWidth); i++) {
      //cout << i << endl;
      BitVector in(inWidth, i);
      sim.setInput(inputName, in);
      sim.evaluate();
      BitVector out = sim.getOutput(outName);

      resultsTable.push_back({in, out});
    }

    auto res = table->caseStatement(table->getValue("in"), resultsTable);

    // auto res = table->functionCall(f->getName(), table->getValue("in"));
    table->assign(table->getValue("out"), res);

    return table;
  }

  FunctionCall::FunctionCall(Function* function_,
                             const std::map<std::string, Value*>& inputs_) :
      Expression(function_->outputValue()->getData()),
      function(function_),
      inputs(inputs_) {}

  std::string FunctionCall::functionName() const {
    return function->getName();
  }

  Value* Function::unsignedDivide(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    std::string divideName = "unsigned_divide_" + std::to_string(a->bitWidth());
    Value* freshValue = makeUniqueValue(a->bitWidth());
    stmt->addStmt(new Assignment(freshValue,
                                        new FunctionCall(getContext().getBuiltin("unsigned_divide", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::multiply(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    //std::string divideName = "multiply_" + std:
    Value* freshValue = makeUniqueValue(a->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("multiply", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::subtract(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    //std::string divideName = "subtract_" + std::to_string(a->bitWidth());
    Value* freshValue = makeUniqueValue(a->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("subtract", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::add(Value* a, Value* b) {
      assert(a != nullptr);
      assert(b != nullptr);

      assert(sameWidth(*a, *b));

      std::string divideName = "add_" + std::to_string(a->bitWidth());
      Value* freshValue = makeUniqueValue(a->bitWidth());
      stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("add", a->bitWidth()), {{"in0", a}, {"in1", b}})));
      return freshValue;
    }

  Value* Function::zeroExtend(const int resWidth, Value* v) {
    //std::string zextName = "zero_extend_" + std::to_string(resWidth);

      Value* freshValue = makeUniqueValue(resWidth);
      stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("zero_extend", resWidth), {{"in", v}})));
      return freshValue;
    }

  Value* Function::shiftLeft(const int shiftValue, Value* v) {
    std::string shiftName = "shift_left_" + std::to_string(shiftValue);

    Value* freshValue = makeUniqueValue(v->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getBuiltin("shift_left", shiftValue), {{"in", v}})));

    return freshValue;
  }

  Value* Function::logicalShiftRight(const int shiftValue, Value* v) {
    std::string shiftName = "logical_shift_right_" + std::to_string(shiftValue);

    Value* freshValue = makeUniqueValue(v->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getBuiltin("logical_shift_right", shiftValue), {{"in", v}})));

    return freshValue;
  }

  Value* Function::invert(Value* v) {
    std::string invName = "invert_" + std::to_string(v->bitWidth());

    Value* freshValue = makeUniqueValue(v->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("invert", v->bitWidth()), {{"in", v}})));

    return freshValue;
  }

  Function* Function::getBuiltin(const std::string& name, const int width) {
    return getContext().getBuiltin(name, width);
  }

  Function* Function::getBuiltinSlice(const int inWidth,
                                      const int end,
                                      const int start) {
    return getContext().getBuiltinSlice(inWidth, end, start);
  }
  
  Value* Function::slice(const int end, const int start, Value* v) {
    std::string shiftName = "slice_" + std::to_string(end) + "_" + std::to_string(start);

    Value* freshValue = makeUniqueValue(end - start + 1);
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getBuiltinSlice(v->bitWidth(), end, start), {{"in", v}})));

    return freshValue;
  }

  Value* Function::leadZeroCount(Value* v) {
    string countName = "count_leading_zeros";
    Value* freshValue = makeUniqueValue(v->bitWidth());
    auto call =
      new FunctionCall(getBuiltin(countName, v->bitWidth()), {{"in", v}});
    stmt->addStmt(new Assignment(freshValue, call));

    return freshValue;
  }

  Value* Function::shiftLeftVariable(Value* v, Value* shiftAmount) {
    assert(v != nullptr);
    assert(shiftAmount != nullptr);

    Value* freshValue = makeUniqueValue(v->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("shift_left_variable", v->bitWidth()), {{"in0", v}, {"in1", shiftAmount}})));

    return freshValue;

  }

  Value* Function::logicalShiftRightVariable(Value* v, Value* shiftAmount) {
    assert(v != nullptr);
    assert(shiftAmount != nullptr);

    Value* freshValue = makeUniqueValue(v->bitWidth());
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("logical_shift_right_variable", v->bitWidth()), {{"in0", v}, {"in1", shiftAmount}})));

    return freshValue;
  }

  void Function::printStmt(const std::string& str) {
    stmt->addStmt(new PrintStatement(str));
  }

  void Function::printStmt(const std::string& str, const std::vector<Expression*>& exprs) {
    stmt->addStmt(new PrintStatement(str, exprs));
  }
  
  Value* Function::equals(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    Value* freshValue = makeUniqueValue(1);
    stmt->addStmt(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("equals", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::equalsExpr(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    return new FunctionCall(getContext().getBuiltin("equals", 1), {{"in0", a}, {"in1", b}});
  }
  
  Function* Context::getBuiltin(const std::string& name, const int width) {
    string fullName = name + "_" + to_string(width);
    if (hasFunction(fullName)) {
      return getFunction(fullName);
    }

    auto newF =
      newFunction(fullName,
                  {{"in0", arrayType(width)}, {"in1", arrayType(32)}},
                  {{"out", arrayType(width)}});

    return newF;
  }
  

  Function* Context::getBuiltinSlice(const int inWidth,
                                     const int end,
                                     const int start) {
    assert(end >= start);

    string fullName = "slice_" + to_string(end) + "_" + to_string(start);
    if (hasFunction(fullName)) {
      return getFunction(fullName);
    }

    auto newF =
      newFunction(fullName,
                  {{"in", arrayType(inWidth)}},
                  {{"out", arrayType(end - start + 1)}});

    return newF;

  }

}
