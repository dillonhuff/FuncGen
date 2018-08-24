#include "language.h"

namespace FuncGen {

  Value* Function::makeUniqueValue(const int width) {
    Value* v = new Value(context.arrayType(width));
    values.insert(v);
    return v;
  }

}
