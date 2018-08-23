#pragma once

#include "language.h"

namespace FuncGen {

  class Simulator {

    const Function& f;
    std::map<std::string, BitVector> variableValues;
    
  public:
    Simulator(const Function& f_) : f(f_) {
      for (auto& v : f.outputMap()) {
        variableValues.insert({v.first, unknown_bv(v.second->bitWidth())});
      }
    }

    BitVector getOutput(const std::string& outputName) const {
      return map_find(outputName, variableValues);
    }

    void setInput(const std::string& inputName, const BitVector& value) {
      variableValues[inputName] = value;
    }

    void evaluateStatement(const Statement& stmt) {
      
    }

    void evaluate() {
      for (auto stmt : f.getStatements()) {
        evaluateStatement(*stmt);
      }
    }
  };
}
