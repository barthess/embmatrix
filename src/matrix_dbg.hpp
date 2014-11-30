#ifndef MATRIX_COMPARE_HPP_
#define MATRIX_COMPARE_HPP_

#include <iostream>
#include <cstdlib>
#include <assert.h>

#include "matrix_osal.hpp"
#include "string.h"

namespace matrix {

static const float EQUAL_F = 0.0001;
static const double EQUAL_D = 0.0001;

/**
 *
 */
template <int m, int n>
bool operator == (const Matrix<unsigned int, m, n> &A,
                  const Matrix<unsigned int, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (A.M[i] != B.M[i])
      return false;
  return true;
}

/**
 *
 */
template <int m, int n>
bool operator == (const Matrix<int, m, n> &A,
                  const Matrix<int, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (A.M[i] != B.M[i])
      return false;
  return true;
}

/**
 *
 */
template <int m, int n>
bool operator == (const Matrix<float, m, n> &A,
                  const Matrix<float, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (fabsf(A.M[i] - B.M[i]) > EQUAL_F)
      return false;
  return true;
}

/**
 *
 */
template <int m, int n>
bool operator == (const Matrix<double, m, n> &A,
                  const Matrix<double, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (fabsf(A.M[i] - B.M[i]) > EQUAL_D)
      return false;
  return true;
}

/**
 *
 */
template <int m, int n>
bool operator != (const Matrix<unsigned int, m, n> &A, const Matrix<unsigned int, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <int m, int n>
bool operator != (const Matrix<int, m, n> &A, const Matrix<int, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <typename T, int m, int n>
void matrix_fancy_print (const Matrix<T, m, n> &A) {
  for (size_t i=0; i<m; i++){
    std::cout << "[";
    for (size_t j=0; j<n; j++){
      std::cout << A.M[i*n + j];
      if (j < n-1)
        std::cout << " ";
    }
    std::cout << "]\n";
  }
  std::cout << "\n";
}

} /* namespace matrix */

#endif /* MATRIX_COMPARE_HPP_ */
