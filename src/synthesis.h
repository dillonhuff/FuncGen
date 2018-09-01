#pragma once

#include "language.h"

namespace FuncGen {

  class SynthesisConstraints {
    
  public:

    void setMaxCycles(const int i) {
    }
  };

  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints);

}
