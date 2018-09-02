#include "synthesis.h"

#include <fstream>

using namespace std;

namespace FuncGen {

  std::string declareWire(Variable* expr) {
    return "\twire [" + to_string(expr->bitWidth() - 1) + ":0] " + expr->getName();
  }

  vector<Function*> neededFunctions(Function* f) {
    return {f};
  }

  void generateStmt(map<Value*, string>& valueNameMap,
                    Statement* stmt,
                    std::ostream& out) {
    if (stmt->type() == STATEMENT_TYPE_PRINT) {
      out << "\t//" << stmt->toString(0) << endl;
      //out << "\t$display(" + stmt->toString(0) + ")" << endl;
    } else if (stmt->type() == STATEMENT_TYPE_ASSIGNMENT) {
      auto assign = static_cast<Assignment*>(stmt);
      auto lhs = assign->getLHS();
      assert(lhs->type() == EXPRESSION_TYPE_VARIABLE);

      //auto rhs = assign->getRHS();

      out << declareWire(static_cast<Variable*>(lhs)) << endl;
      
      //out << "wire [" << endl;
    } else {
      out << "//" << stmt->toString(0) << endl;
    }
  }
  
  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints) {
    ofstream out(f->getName() + ".v");

    vector<Function*> nf =
      neededFunctions(f);

    map<Value*, string> valueNameMap;
    
    for (auto f : nf) {
      out << "module " + f->getName() + "(";
      int i = 0;
      int numPorts = f->inputMap().size() + f->outputMap().size();
      for (auto in : f->inputMap()) {
        out << "input [" << (in.second->bitWidth() - 1) << ":" << 0 << "] " << in.first << ", ";
        valueNameMap[in.second] = in.first;
        i++;
      }
      for (auto outP : f->outputMap()) {
        out << "output [" << (outP.second->bitWidth() - 1) << ":" << 0 << "] " << outP.first;
        if (i < (numPorts - 1)) {
          out << ", ";
        }
        valueNameMap[outP.second] = outP.first;
        i++;
      }

      out << ");" << endl;

      for (auto stmt : f->getStatements()) {
        generateStmt(valueNameMap, stmt, out);
        //out << "// " << stmt->toString(1) << endl;
      }

      out << "endmodule" << endl;
    }
    

    out.close();
  }
}
