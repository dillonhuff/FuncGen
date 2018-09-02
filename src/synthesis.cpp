#include "synthesis.h"

#include "algorithm.h"

#include <fstream>

using namespace std;
using namespace dbhc;

namespace FuncGen {

#define sc static_cast

  typedef std::map<Value*, string> ValMap;

  class CodeGenState {
    unsigned uniqueNum;
    
  public:

    ValMap valueNameMap;

    CodeGenState() : uniqueNum(0) {}

    std::string uniqueName() {
      return uniqueName("synth_var");
    }

    std::string uniqueName(const std::string& prefix) {
      auto num = uniqueNum;
      uniqueNum++;
      return prefix + "_" + to_string(num);
    }

  };

  std::string getWire(Value* expr,
                      CodeGenState& state,
                      ostream& out) {
    if (contains_key(expr, state.valueNameMap)) {
      return map_find(expr, state.valueNameMap);
    } else {
      assert(expr->type() == EXPRESSION_TYPE_VARIABLE);
      auto var = static_cast<Variable*>(expr);
      out << "\twire [" + to_string(var->bitWidth() - 1) + ":0] " + var->getName() << ";" << endl;
      state.valueNameMap.insert({var, var->getName()});
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

  void assignVars(const std::string& lhs, const std::string& rhs, ostream& out) {
    out << "\tassign " << lhs << " = " << rhs << ";" << endl;
  }

  std::vector<std::string> portConnections(map<string, string>& conns) {
    vector<string> strs;
    for (auto conn : conns) {
      strs.push_back("." + conn.first + "(" + conn.second + ")");
    }
    return strs;
  }

  std::string commaList(const std::vector<std::string>& strs) {
    string str;
    for (int i = 0; i < (int) strs.size(); i++) {
      str += strs[i];
      if (i < (((int) strs.size()) - 1)) {
        str += ", ";
      }
    }
    return str;
  }
  
  string generateExpression(Expression* expr,
                            CodeGenState& state,
                            ostream& out) {
    if (expr->type() == EXPRESSION_TYPE_VARIABLE) {
      auto var = sc<Variable*>(expr);
      return var->getName();
    } else if (expr->type() == EXPRESSION_TYPE_FUNCTION_CALL) {
      auto fc = sc<FunctionCall*>(expr);

      map<string, string> inputMap;
      for (auto in : fc->getInputs()) {
        string name = state.uniqueName(fc->functionName() + "_" + in.first);
        out << "\twire [" << (in.second->bitWidth() - 1) << ":0] " << name << ";" << endl;

        string r = generateExpression(in.second, state, out);
        assignVars(name, r, out);
        inputMap.insert({in.first, name});
      }

      string outName = fc->outputValueName();
      string outWire = state.uniqueName(outName);

      inputMap.insert({outName, outWire});

      out << "\t" << fc->functionName() << " " << state.uniqueName(fc->functionName()) << "(" << commaList(portConnections(inputMap)) << ");" << endl;
      return outWire;
    } else if (expr->type() == EXPRESSION_TYPE_CONSTANT) {
      auto c = sc<ConstantValue*>(expr);
      auto bv = c->getValue();
      return bv.hex_string();
    } else {
      assert(false);
    }
  }

  void generateStmt(CodeGenState& state,
                    Statement* stmt,
                    std::ostream& out) {
    if (stmt->type() == STATEMENT_TYPE_PRINT) {
      out << "\t//" << stmt->toString(0) << endl;
    } else if (stmt->type() == STATEMENT_TYPE_ASSIGNMENT) {
      auto assign = static_cast<Assignment*>(stmt);
      auto lhs = assign->getLHS();
      assert(lhs->type() == EXPRESSION_TYPE_VARIABLE);

      auto rhs = assign->getRHS();

      string name = getWire(static_cast<Variable*>(lhs), state, out);
      string res = generateExpression(rhs, state, out);
      assignVars(name, res, out);

    } else {
      out << "//" << stmt->toString(0) << endl;
    }
  }
  
  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints) {
    ofstream out(f->getName() + ".v");

    vector<Function*> nf =
      neededFunctions(f);

    CodeGenState state;
    //map<Value*, string> valueNameMap;
    
    for (auto f : nf) {
      out << "module " + f->getName() + "(";
      int i = 0;
      int numPorts = f->inputMap().size() + f->outputMap().size();
      for (auto in : f->inputMap()) {
        out << "input [" << (in.second->bitWidth() - 1) << ":" << 0 << "] " << in.first << ", ";
        state.valueNameMap[in.second] = in.first;
        i++;
      }
      for (auto outP : f->outputMap()) {
        out << "output [" << (outP.second->bitWidth() - 1) << ":" << 0 << "] " << outP.first;
        if (i < (numPorts - 1)) {
          out << ", ";
        }
        state.valueNameMap[outP.second] = outP.first;
        i++;
      }

      out << ");" << endl;

      for (auto stmt : f->getStatements()) {
        generateStmt(state, stmt, out);
        //out << "// " << stmt->toString(1) << endl;
      }

      out << "endmodule" << endl;
    }
    

    out.close();
  }
}
