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

}
