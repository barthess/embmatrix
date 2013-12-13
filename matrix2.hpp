#ifndef MATRIX2_H
#define MATRIX2_H

#include "platform.h"
#include "string.h"
#include "matrix_primitives.hpp"

template<typename T, int r, int c>
class Matrix2 {

public:
  T m[r*c];
//  size_t row = r;
//  size_t col = c;

public:
  /**
   *
   */
  Matrix2(void) {
    matrixDbgPrint("Matrix default constructor\n");
    return;
  }

  /**
   *
   */
  Matrix2(const Matrix2 &src){
    matrixDbgPrint("Matrix copy constructor\n");
    memcpy(this->m, src.m, sizeof(m));
  }

  /**
   *
   */
  Matrix2(T pattern) {
    matrixDbgPrint("Matrix pattern constructor\n");
    for (size_t i=0; i<(c*r); i++)
      this->m[i] = pattern;
  }

  /**
   *
   */
  Matrix2(const T *array, size_t arraysize){
    matrixDbgPrint("Matrix array initialization constructor\n");
    matrixDbgCheck(sizeof(m) == arraysize, "sizes mismatch");
    memcpy(this->m, array, sizeof(m));
  }

  /**
   * @brief Assign operator
   */
  Matrix2& operator=(const Matrix2 &src){
    matrixDbgPrint("Matrix assign operator\n");

    if (this == &src)
      return *this;

    /* */
    memcpy(this->m, src.m, sizeof(m));
    return *this;
  }

  /**
   * @note    here is no need to check sizes at run time
   */
  Matrix2 operator + (const Matrix2 &S){
    matrixDbgPrint("Matrix + operator\n");
    Matrix2<T, r, c> ret;

    for (size_t i=0; i<(r*c); i++){
      ret.m[i] = this->m[i] + S.m[i];
    }
    return ret;
  }


//  /**
//   * @brief   Multiply matrix themself by another matrix
//   * @param[in] right     multiplier
//   * @param[out] result   place result in it
//   */
//  void mul(const Matrix2 &right, Matrix2 &result){

//    matrixDbgCheck(((this->col == right.row) &&
//                (result.row == this->row) &&
//                (result.col == right.col)), "sizes inconsistent");

//    matrix_multiply(this->row, this->col, right.col,
//                    this->m, right.m, result.m);
//  }



//  /**
//   * @brief   Multiply operator
//   * @param[in] right     multiplier
//   */
//  Matrix2 operator * (const Matrix2 &right) {
//    Matrix2<T, this->row, right.col> ret;
//    this->mul(right, ret);
//    return ret;
//  }





  /**
   * @note    here is no need to check sizes at run time
   */
  void operator += (const Matrix2 &S){
    matrixDbgPrint("Matrix += operator\n");
    for (size_t i=0; i<(r*c); i++){
      m[i] += S.m[i];
    }
  }

  /**
   *
   */
  void print(void){
    matrixDbgPrint(c);
    matrixDbgPrint("x");
    matrixDbgPrint(r);
    matrixDbgPrint(":\n");
    for (size_t i=0; i<r; i++){
      for (size_t j=0; j<c; j++){
        matrixDbgPrint(m[i*r + j]);
        matrixDbgPrint(" ");
      }
      matrixDbgPrint("\n");
    }
  }
};


template <typename T, int m, int n, int p>
Matrix2<T, m, p> operator * (const Matrix2<T, m, n> &left, const Matrix2<T, n, p> &right) {
  matrixDbgPrint("Matrix * operator\n");
  Matrix2<T, m, p> ret;
  ret.m[0] = left.m[0] * right.m[0];
  return ret;
}

//template <typename T, int r, int c, int rnew>
//Matrix2<T, rnew, c> submatrix(Matrix2<T, r, c> &left) {
//  Matrix2<T, rnew, c> ret;
//  ret.m = left.m + r;
//  return ret;
//}

#endif // MATRIX2_H















