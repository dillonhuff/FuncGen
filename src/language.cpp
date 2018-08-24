#include "language.h"

using namespace std;

namespace FuncGen {

  Value* Function::makeUniqueValue(const int width) {
    Value* v = new Value(context.arrayType(width));
    string name = "av_" + std::to_string(uniqueNum);
    uniqueNum++;
    values.insert({name, v});
    return v;
  }

  Value* Function::addConstant(const int bitWidth, const int value) {
    ConstantValue* v = new ConstantValue(context.arrayType(bitWidth),
                                         BitVector(bitWidth, value));
                                           
    values.insert({"const_" + std::to_string(uniqueNum), v});
    uniqueNum++;
    return v;
  }

  Value* Function::functionCall(const std::string& str,
                                const std::map<std::string, Value*>& args) {
    Function* f = context.getFunction(str);
    Value* freshValue = makeUniqueValue(f->outputValue()->bitWidth());
    statements.push_back(new FunctionCall(str, freshValue, args));
    return freshValue;
  }

  Value* Function::functionCall(const std::string& str, Value* arg) {
    Function* f = context.getFunction(str);
    assert(f->inputMap().size() == 1);

    string argName = begin(f->inputMap())->first;

    Value* freshValue = makeUniqueValue(f->outputValue()->bitWidth());
    statements.push_back(new FunctionCall(str, freshValue, {{argName, arg}}));
    return freshValue;
    
  }

  Function*
  precomputeTable(const std::string& tableName, Function* f, Context& c) {
    assert(f->inputMap().size() == 1);
    assert(f->outputMap().size() == 1);

    int inWidth = (*begin(f->inputMap())).second->bitWidth();
    int outWidth = (*begin(f->outputMap())).second->bitWidth();

    Function* table = c.newFunction(tableName,
                                    {{"in", c.arrayType(inWidth)}},
                                    {{"out", c.arrayType(outWidth)}});

    auto res = table->functionCall(f->getName(), table->getValue("in"));
    table->assign(table->getValue("out"), res);

    return table;
  }
}
