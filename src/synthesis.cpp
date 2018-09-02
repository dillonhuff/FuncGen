#include "synthesis.h"

#include "algorithm.h"

#include <fstream>

using namespace std;
using namespace dbhc;

namespace FuncGen {

  std::string getWire(Value* expr,
                      map<Value*, string>& valueNameMap,
                      ostream& out) {
    if (contains_key(expr, valueNameMap)) {
      return map_find(expr, valueNameMap);
    } else {
      assert(expr->type() == EXPRESSION_TYPE_VARIABLE);
      auto var = static_cast<Variable*>(expr);
      out << "\twire [" + to_string(var->bitWidth() - 1) + ":0] " + var->getName() << ";" << endl;
      valueNameMap.insert({var, var->getName()});
      return var->getName();
    }
  }

  vector<Function*> neededFunctions(Statement* s) {
    vector<Function*> needed;
    if (s->type() == STATEMENT_TYPE_ASSIGNMENT) {
      auto rhs = static_cast<Assignment*>(s)->getRHS();
      if (rhs->type() == EXPRESSION_TYPE_FUNCTION_CALL) {
        FunctionCall* f = static_cast<FunctionCall*>(rhs);
        needed.push_back(f->getFunction());
      }
    }
    return needed;
  }

  vector<Function*> neededFunctions(Function* f) {
    vector<Function*> needed{f};
    for (auto stmt : f->getStatements()) {
      for (auto nf : neededFunctions(stmt)) {
        if (!elem(nf, needed)) {
          needed.push_back(nf);
        }
      }
    }
    reverse(needed);

    return needed;
  }

  void generateStmt(map<Value*, string>& valueNameMap,
                    Statement* stmt,
                    std::ostream& out) {
    if (stmt->type() == STATEMENT_TYPE_PRINT) {
      out << "\t//" << stmt->toString(0) << endl;
    } else if (stmt->type() == STATEMENT_TYPE_ASSIGNMENT) {
      auto assign = static_cast<Assignment*>(stmt);
      auto lhs = assign->getLHS();
      assert(lhs->type() == EXPRESSION_TYPE_VARIABLE);

      //auto rhs = assign->getRHS();

      string name = getWire(static_cast<Variable*>(lhs), valueNameMap, out);
      //out << declareWire(static_cast<Variable*>(lhs)) << endl;
      
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
