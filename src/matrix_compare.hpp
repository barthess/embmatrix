#ifndef MATRIX_COMPARE_HPP_
#define MATRIX_COMPARE_HPP_

#include "matrix_osal.hpp"
#include "string.h"

namespace matrix {

template <int m, int n>
bool operator == (const Matrix<unsigned int, m, n> &A, const Matrix<unsigned int, m, n> &B) {
  return 0 == memcmp(A.M, B.M, sizeof(A.M[0]) * m * n);
}

template <int m, int n>
bool operator == (const Matrix<int, m, n> &A, const Matrix<int, m, n> &B) {
  return 0 == memcmp(A.M, B.M, sizeof(A.M[0]) * m * n);
}

//template <int m, int n>
//bool operator == (const Matrix<float, m, n> &A, const Matrix<float, m, n> &B) {
//  if (0 == memcmp(A.M, B.M, sizeof(A.M[0]) * m * n))
//    return true;
//  else
//    return false;
//}
//
//template <int m, int n>
//bool operator == (const Matrix<double, m, n> &A, const Matrix<double, m, n> &B) {
//  if (0 == memcmp(A.M, B.M, sizeof(A.M[0]) * m * n))
//    return true;
//  else
//    return false;
//}

template <int m, int n>
bool operator != (const Matrix<unsigned int, m, n> &A, const Matrix<unsigned int, m, n> &B) {
  return !(A == B);
}

template <int m, int n>
bool operator != (const Matrix<int, m, n> &A, const Matrix<int, m, n> &B) {
  return !(A == B);
}

} /* namespace matrix */

#endif /* MATRIX_COMPARE_HPP_ */
