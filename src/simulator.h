#pragma once

#include "language.h"

namespace FuncGen {

  class Simulator {
    
  public:
    Simulator(const Function& f) {
      
    }

    BitVector getOutput(const std::string& outputName) const {
      assert(false);
    }

    void setInput(const std::string& inputName, const BitVector& value) {
      
    }

    void evaluate() {
    }
  };
}
