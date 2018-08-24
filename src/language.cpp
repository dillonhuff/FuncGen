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
}
