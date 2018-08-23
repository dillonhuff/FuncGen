#pragma once

#include "language.h"

namespace FuncGen {

  class Simulator {

    std::map<std::string, BitVector> variableValues;
    
  public:
    Simulator(const Function& f) {
      for (auto& v : f.outputMap()) {
        variableValues.insert({v.first, unknown_bv(v.second->bitWidth())});
      }
    }

    BitVector getOutput(const std::string& outputName) const {
      return map_find(outputName, variableValues);
    }

    void setInput(const std::string& inputName, const BitVector& value) {
      
    }

    void evaluate() {
    }
  };
}
