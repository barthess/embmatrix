#include "hal.h"
#include "matrix.hpp"
#include "matrix_mempool.hpp"

#include "kalman.hpp"

using namespace matrix;

static uint64_t matrix_total_time = 0;

static const klmfp init[] = {1,2,3, 4,5,6, 7,8,9, 10,11,12, 10,11,12,13,
    1,2,3, 4,5,6, 7,8,9, 10,11,12, 10,11,12,13};
//static const float init[] = {1,2,3, 4,5,6, 7,8,9,};
//static const float init[] = {1,2,3};

/**
 *
 */
Kalman::Kalman(void) :
  R(0.5), F(0.7), T(0.8), J(1), Ttest(2.3), RTtest(init, sizeof(init))
{
  return;
}


float Kalman::run(void) {

  Matrix<klmfp, 3, 3> Patch(0);

  for(size_t i=0; i<100; i++){
    uint32_t start = chSysGetRealtimeCounterX();
    R = T * J * F * ~T;
    R = R * F;
    J = R + T;
    patch(F, Patch, 3, 1);
    patch(F, row(Patch, 0), 10, 11);
    //RTtest = !J * ~Ttest;
    RTtest = J * ~Ttest;
    matrix_total_time += chSysGetRealtimeCounterX() - start;
  }
  chThdSleepMilliseconds(1); /* hack to enforce stack check by chibios */

  matrix_alloc_time = 0;
  matrix_free_time = 0;
  matrix_total_time = 0;

  return 0;
}


