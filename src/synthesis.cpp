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

  vector<Function*> neededFunctions(Function* f);
  
  vector<Function*> neededFunctions(Expression* rhs) {
    vector<Function*> needed;

    if (rhs->type() == EXPRESSION_TYPE_FUNCTION_CALL) {
      
      FunctionCall* f = static_cast<FunctionCall*>(rhs);
      for (auto in : f->getInputs()) {
        concat(needed, neededFunctions(in.second));
      }
      concat(needed, neededFunctions(f->getFunction()));
      needed.push_back(f->getFunction());
    } else {
    }

    return needed;
  }

  vector<Function*> neededFunctions(Statement* s) {
    vector<Function*> needed;
    if (s->type() == STATEMENT_TYPE_ASSIGNMENT) {
      auto rhs = static_cast<Assignment*>(s)->getRHS();
      concat(needed, neededFunctions(rhs));
    } else if (s->type() == STATEMENT_TYPE_CASE) {
      Case* c = sc<Case*>(s);
      for (auto c : c->getCases()) {
        concat(needed, neededFunctions(c.second));
      }
    } else if (s->type() == STATEMENT_TYPE_REPEAT) {
      RepeatStmt* r = sc<RepeatStmt*>(s);
      concat(needed, neededFunctions(r->body()));
    } else {
    }

    return needed;
  }

  vector<Function*> neededFunctions(Function* f) {
    vector<Function*> needed{f};
    for (auto stmt : f->getStatements()) {
      for (auto nf : neededFunctions(stmt)) {
        cout << "Need " << nf->getName() << endl;
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

  string wireDecl(const std::string& name, int width) {
    return "\twire [" + to_string(width - 1) + ":0] " + name + ";";
  }
  
  string generateExpression(Expression* expr,
                            CodeGenState& state,
                            ostream& out) {
    if (expr->type() == EXPRESSION_TYPE_VARIABLE) {
      return getWire(expr, state, out);
      // auto var = sc<Variable*>(expr);
      // return var->getName();
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

      out << wireDecl(outWire, fc->outputWidth()) << endl;

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

    } else if (stmt->type() == STATEMENT_TYPE_CASE) {
      auto cs = sc<Case*>(stmt);
      
      string switchVar = generateExpression(cs->getInput(), state, out);
      string outVar = generateExpression(cs->getResult(), state, out);
      string outVarReg = outVar + "reg";

      out << "\treg [" << (cs->getResult()->bitWidth() - 1) << ":0] " << outVarReg << "; " << endl;

      vector<pair<BitVector, string> > evaluatedCases;
      for (auto c : cs->getCases()) {
        string res = generateExpression(c.second, state, out);
        evaluatedCases.push_back({c.first, res});
      }

      out << "\talways @(*) begin\n";
      out << "\t\tcase (" << switchVar << ") " << endl;

      for (auto cs : evaluatedCases) {
        out << "\t\t\t" << cs.first.hex_string() << ": " << outVarReg << " = " << cs.second << ";" << endl;
      }
      out << "\t\tendcase" << endl;
      out << "\tend\n";

      assignVars(outVar, outVarReg, out);
    } else if (stmt->type() == STATEMENT_TYPE_REPEAT) {
      auto rp = sc<RepeatStmt*>(stmt);
      int numIters = rp->numIterations();
      Statement* body = rp->body();

      assert(body->type() == STATEMENT_TYPE_ASSIGNMENT);
      auto innerAssign = sc<Assignment*>(body);
      Value* v = innerAssign->getLHS();
      assert(v->type() == EXPRESSION_TYPE_VARIABLE);
      Variable* var = sc<Variable*>(v);

      //string updateVarName = map_find(v, state.valueNameMap);
      // string name = state.uniqueName(var->getName());
      // out << wireDecl(name, var->bitWidth());
      // state.valueNameMap[v] = name;

      Value* rhs = innerAssign->getRHS();
      for (int i = 0; i < numIters; i++) {
        out << "\t// ### Iteration " << i << endl;

        string res = generateExpression(rhs, state, out);

        string newName = state.uniqueName(var->getName());
        out << wireDecl(newName, var->bitWidth()) << endl;
        state.valueNameMap[v] = newName;
        
        //assignVars(newName, updateVarName, out);
        //assignVars(newName, updateVarName, out);
        assignVars(newName, res, out);
        //updateVarName = newName;

        out << "\t// #########################" << endl;
      }
    } else {
      assert(false);
      //out << "//" << stmt->toString(0) << endl;
    }
  }

  bool isBuiltin(Function* f) {
    string name = f->getName();
    vector<string> prefixes({"equals_", "slice_", "subtract_", "logical_shift_right", "arithmetic_shift_right_", "shift_left_", "zero_extend_", "multiply_", "add_", "invert_", "count_leading_zeros_"});
    for (auto prefix : prefixes) {
      if (hasPrefix(name, prefix)) {
        return true;
      }
    }

    return false;
  }

  string binopDef(const std::string& op) {
    return "\tassign out = in0 " + op + " in1;\n";
  }

  string builtinDef(Function* f) {
    string name = f->getName();
    if (hasPrefix(name, "add_")) {
      return binopDef("+");
    }
    if (hasPrefix(name, "subtract_")) {
      return binopDef("-");
    }
    if (hasPrefix(name, "multiply_")) {
      return binopDef("*");
    }
    if (hasPrefix(name, "equals_")) {
      return binopDef("==");
    }
    if (hasPrefix(name, "shift_left_")) {
      return binopDef("<<");
    }
    if (hasPrefix(name, "logical_shift_right_variable_")) {
      return binopDef(">>");
    }
    if (hasPrefix(name, "logical_shift_right_")) {
      std::string pre = "logical_shift_right_";
      int shiftAmount = stoi(name.substr(pre.size()));

      return "\tassign out = in >> " + to_string(shiftAmount) + " ;\n";
    }

    if (hasPrefix(name, "arithmetic_shift_right_")) {
      return binopDef(">>>");
    }
    if (hasPrefix(name, "invert_")) {
      return "\tassign out = ~in;\n";
    }

    if (hasPrefix(name, "count_leading_zeros_")) {
        std::string pre = "count_leading_zeros_";

        int width = stoi(name.substr(pre.size()));

        string str = "";
        str += "\treg [" + to_string(width - 1) + ":0] out_reg;\n";
        str += "\talways @(*) begin\n";
        str += "\t\tcasez(in)\n";
        for (int i = 0; i < width; i++) {
          string inPattern = "";
          for (int pre = 0; pre < i; pre++) {
            inPattern += '0';
          }
          inPattern += '1';
          for (int post = i + 1; post < width; post++) {
            inPattern += '?';
          }

          string res = to_string(i);
          str += "\t\t\t" + to_string(width) + "'b" + inPattern + ": out_reg = " + to_string(i) + ";\n";
        }
        str += "\t\tendcase\n";
        str += "\tend\n";
        str += "\tassign out = out_reg;\n";
        return str;
    }

    if (hasPrefix(name, "slice_")) {
        std::string pre = "slice_";

        size_t sz;
        int endSlice = stoi(name.substr(pre.size()), &sz);

        int startSlice = stoi(name.substr(pre.size() + sz + 1));

        return "\tassign out = in[" + to_string(endSlice) + ":" + to_string(startSlice) + "];\n";
    }

    if (hasPrefix(name, "zero_extend_")) {
      std::string pre = "zero_extend_";

      size_t sz;
      int outWidth = stoi(name.substr(pre.size()), &sz);

      int inWidth = stoi(name.substr(pre.size() + sz + 1));

      assert(outWidth >= inWidth);

      return "\tassign out = {{(" + to_string(outWidth) + " - " + to_string(inWidth) + "){1'b0}}, in};\n";
    }


    
    
    return "\t//insert definition\n";
  }
  
  void synthesizeVerilog(Function* f, const SynthesisConstraints& constraints) {
    ofstream out(f->getName() + ".v");

    vector<Function*> nf =
      neededFunctions(f);

    CodeGenState state;

    out << "/* verilator lint_off UNUSED */" << endl;
    
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

      if (isBuiltin(f)) {
        assert(f->getStatements().size() == 0);
        out << builtinDef(f);
      }  else {
      
        for (auto stmt : f->getStatements()) {
          generateStmt(state, stmt, out);
        }
      }

      out << "endmodule" << endl << endl;
    }
    

    out.close();
  }
}
