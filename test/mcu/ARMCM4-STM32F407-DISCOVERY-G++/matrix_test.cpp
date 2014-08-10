#include "ch.hpp"
#include "hal.h"
#include "matrix.hpp"

using namespace matrix;

static Matrix<float, 12, 12> R(0.5), F(0.7), T(0.8), J(1);

static Matrix<float, 1, 12> Ttest(2.3);
static const float init[] = {1,2,3, 4,5,6, 7,8,9, 10,11,12};
static Matrix<float, 12, 1> RTtest(init, sizeof(init));


float *matrix_test(void) {
  R = T * J;
  R = R * T;
  J = R + T;
  RTtest = !J * ~Ttest;

  chThdSleep(1); /* hack to enforce stack check by chibios */
  return R.M;
}


