#include "ch.hpp"
#include "hal.h"
#include "matrix.hpp"

using namespace matrix;

static Matrix<float, 12, 12> R(0), F(0), T(0), J(0);

static const float init[] = {1,2,3, 4,5,6, 7,8,9};
static Matrix<float, 3, 3> R3x3(init, sizeof(init));


float matrix_test(void) {
  R = T * J;
  R = R * T;
  J = R + T;

  chThdSleep(1);
  //return R(0, 0);
  return 0;
}


