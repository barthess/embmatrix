#ifndef MATRIX_COMPARE_HPP_
#define MATRIX_COMPARE_HPP_

#include <iostream>
#include <cstdlib>
#include <assert.h>

#include "matrix_osal.hpp"
#include "string.h"

namespace matrix {

static const float EPSILON_F = 0.001;
static const double EPSILON_D = 0.001;

/**
 *
 */
template <size_t m, size_t n>
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
template <size_t m, size_t n>
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
template <size_t m, size_t n>
bool operator == (const Matrix<float, m, n> &A,
                  const Matrix<float, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (fabsf(A.M[i] - B.M[i]) > EPSILON_F)
      return false;
  return true;
}

/**
 *
 */
template <size_t m, size_t n>
bool operator == (const Matrix<double, m, n> &A,
                  const Matrix<double, m, n> &B) {
  size_t i = n * m;

  while(i--)
    if (fabsf(A.M[i] - B.M[i]) > EPSILON_D)
      return false;
  return true;
}

/**
 *
 */
template <size_t m, size_t n>
bool operator != (const Matrix<unsigned int, m, n> &A,
                  const Matrix<unsigned int, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <size_t m, size_t n>
bool operator != (const Matrix<int, m, n> &A,
                  const Matrix<int, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <size_t m, size_t n>
bool operator != (const Matrix<float, m, n> &A,
                  const Matrix<float, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <size_t m, size_t n>
bool operator != (const Matrix<double, m, n> &A,
                  const Matrix<double, m, n> &B) {
  return !(A == B);
}

/**
 *
 */
template <typename T, size_t m, size_t n>
void matrix_fancy_print (const Matrix<T, m, n> &A) {

  if (A.tr) {
    matrixDbgPrint("Matrix print cheat\n");
    /* little cheat for transposed matrix */
    Matrix<T, m, n> B;
    matrix_deep_transpose(n, m, A.M, B.M);
    matrix_fancy_print(B);
  }
  else {
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
}

} /* namespace matrix */

#endif /* MATRIX_COMPARE_HPP_ */
