#include "Vnewton_raphson_divide_16.h"
#include "verilated.h"

int main() {
  Vnewton_raphson_divide_16 top;
  top.N = 8;
  top.D = 2;

  top.eval();

  assert(top.Q == (8 / 2));
}
