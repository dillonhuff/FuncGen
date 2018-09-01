#include "synthesis.h"

#include <fstream>

using namespace std;

namespace FuncGen {

  vector<Function*> neededFunctions(Function* f) {
    return {f};
  }
  
  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints) {
    ofstream out(f->getName() + ".v");

    vector<Function*> nf =
      neededFunctions(f);

    for (auto f : nf) {
      out << "module " + f->getName() + "(";
      int i = 0;
      int numPorts = f->inputMap().size() + f->outputMap().size();
      for (auto in : f->inputMap()) {
        out << "input [" << (in.second->bitWidth() - 1) << ":" << 0 << "] " << in.first << ", ";
        i++;
      }
      for (auto outP : f->outputMap()) {
        out << "output [" << (outP.second->bitWidth() - 1) << ":" << 0 << "] " << outP.first;
        if (i < (numPorts - 1)) {
          out << ", ";
        }
        i++;
      }

      out << "); endmodule" << endl;
    }
    

    out.close();
  }
}
