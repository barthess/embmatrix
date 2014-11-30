#include <iostream>

#include "string.h"
#include "matrix.hpp"
#include "kalman.hpp"
#include "matrix_osal_pc.cpp"
#include "matrix_dbg.hpp"

using namespace matrix;

uint32_t matrix_alloc_time = 0;
uint32_t matrix_free_time = 0;

size_t matrix_malloc_cnt = 0;
size_t matrix_free_cnt = 0;

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
  std::cout << "-----\n";
  for(size_t i=0; i<1; i++){
    //R = T * J * F * ~(T*T);
    //std::cout << "-----\n";
    //R = R * F;
    // J = R + T;
    // RTtest = J * ~Ttest;
  }

  return 0;
}

/**
 * @brief main
 * @return
 */

Kalman *kalman = new Kalman();

static const unsigned int vert_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void){
  kalman->run();

  Matrix<unsigned int, 5, 2> A(vert_array, sizeof(vert_array));
  Matrix<unsigned int, 2, 5> B(vert_array, sizeof(vert_array));
  Matrix<unsigned int, 2, 2> C;
  std::cout << "-----\n";
  //B = ~A;
  C = ~A * ~B;
  matrix_fancy_print(C);
}
