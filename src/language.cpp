#include "language.h"
#include "simulator.h"

using namespace std;

namespace FuncGen {

  Value*
  Function::caseStatement(Value* in, const std::vector<std::pair<BitVector, BitVector> >& cases) {
    assert(cases.size() > 0);
    int resWidth = cases.at(0).second.bitLength();
    Value* freshValue = makeUniqueValue(resWidth);

    Cases caseExprs;
    for (auto c : cases) {
      caseExprs.push_back({c.first, constant(c.second)});
    }
    statements.push_back(new Case(freshValue, in, caseExprs));
    return freshValue;
  }

  Value*
  Function::caseStatement(Value* in, const Cases& cases) {
    assert(cases.size() > 0);

    int resWidth = cases.at(0).second->bitWidth();
    Value* freshValue = makeUniqueValue(resWidth);
    statements.push_back(new Case(freshValue, in, cases));

    return freshValue;
  }
  
  Value* Function::makeUniqueValue(const int width) {
    Value* v = new Value(context.arrayType(width));
    string name = "av_" + std::to_string(uniqueNum);
    uniqueNum++;
    values.insert({name, v});
    return v;
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
    statements.push_back(new Assignment(freshValue, new FunctionCall(f, args)));
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
    statements.push_back(new Assignment(freshValue, new FunctionCall(f, {{argName, arg}})));
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
    statements.push_back(new Assignment(freshValue,
                                        new FunctionCall(getContext().getBuiltin("unsigned_divide", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::multiply(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    //std::string divideName = "multiply_" + std:
    Value* freshValue = makeUniqueValue(a->bitWidth());
    statements.push_back(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("multiply", a->getBitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }

  Value* Function::subtract(Value* a, Value* b) {
    assert(a != nullptr);
    assert(b != nullptr);

    assert(sameWidth(*a, *b));

    //std::string divideName = "subtract_" + std::to_string(a->bitWidth());
    Value* freshValue = makeUniqueValue(a->bitWidth());
    statements.push_back(new Assignment(freshValue, new FunctionCall(getContext().getBuiltin("subtract", a->bitWidth()), {{"in0", a}, {"in1", b}})));
    return freshValue;
  }
  
}
