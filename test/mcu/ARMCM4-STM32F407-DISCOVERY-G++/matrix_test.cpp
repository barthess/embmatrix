#include "ch.hpp"
#include "hal.h"
#include "matrix2.hpp"
#include "matrix3.hpp"

using namespace matrix;

static Matrix2<float, 12, 12> A(0), B(0), C(0), D(0);

float* matrix_test(void) {
  A = B * C;
  chThdSleep(1);
  return A.M;
}

static Matrix3<float, 12, 12> R(0), F(0), T(0), J(0);

static const float init[] = {1,2,3, 4,5,6, 7,8,9};
static Matrix3<float, 3, 3> R3x3(init, sizeof(init));


float matrix3_test(void) {
  R = T * J;
  R = R * T;
  J = R + T;

  chThdSleep(1);
  //return R(0, 0);
  return 0;
}


