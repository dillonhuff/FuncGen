#include "synthesis.h"

#include <fstream>

using namespace std;

namespace FuncGen {

  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints) {
    ofstream out(f->getName() + ".v");
    out << "module " + f->getName() + "(); endmodule" << endl;
    out.close();
  }
}
