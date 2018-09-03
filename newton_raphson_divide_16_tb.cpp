#include "Vnewton_raphson_divide_16.h"
#include "verilated.h"

#include <iostream>

using namespace std;

int main() {
  Vnewton_raphson_divide_16 top;
  top.N = 8;
  top.D = 2;

  top.eval();

  assert(top.Q == (8 / 2));

  top.N = -8;
  top.D = 2;

  top.eval();

  cout << "-8 / 2 = " << (-8 / 2) << endl;
  cout << "top.Q  = " << (int) top.Q << endl;

  assert(top.Q == ((-8 / 2) & 0xffff));


  top.N = 8;
  top.D = 3;

  top.eval();

  assert(top.Q == (8 / 3));

  top.N = 239;
  top.D = 17;

  top.eval();

  cout << "top.Q      = " << (int) top.Q << endl;
  cout << "(239 / 17) = " << (239 / 17) << endl;

  assert(top.Q == (239 / 17));

  top.N = 239;
  top.D = -17;

  top.eval();

  cout << "top.Q       = " << (int) top.Q << endl;
  cout << "(239 / -17) = " << (239 / -17) << endl;

  assert(top.Q == ((239 / -17) & 0xffff));

  top.N = 20;
  top.D = 5;

  top.eval();
  
  cout << "top.Q      = " << (int) top.Q << endl;
  cout << "(20 / 5)   = " << (20 / 5) << endl;
}
