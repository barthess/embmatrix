#include "ch.hpp"
#include "hal.h"
#include "matrix2.hpp"

using namespace matrix;

static Matrix2<float, 12, 12> A(0), B(0), C(0), D(0);

float* matrix_test(void) {
  A = B * C;
  chThdSleep(1);
  return A.M;
}


