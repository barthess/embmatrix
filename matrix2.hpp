#ifndef MATRIX2_H
#define MATRIX2_H

#include "platform.h"
#include "string.h"
#include "matrix_primitives.hpp"

namespace matrix {

template<typename T, int r, int c>
class Matrix2 {

public:
  T M[r*c];
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
    memcpy(this->M, src.M, sizeof(M));
  }

  /**
   *
   */
  template<int n>
  Matrix2(const Matrix2<T, r, n> &left, const Matrix2<T, n, c> &right){
    matrixDbgPrint("Matrix multiply constructor\n");
    matrix_multiply<T>(r, n, c, left.M, right.M, this->M);
  }

  /**
   *
   */
  Matrix2(T pattern) {
    matrixDbgPrint("Matrix pattern constructor\n");
    for (size_t i=0; i<(c*r); i++)
      this->M[i] = pattern;
  }

  /**
   *
   */
  Matrix2(const T *array, size_t arraysize){
    matrixDbgPrint("Matrix array initialization constructor\n");
    matrixDbgCheck(sizeof(M) == arraysize, "sizes mismatch");
    memcpy(this->M, array, sizeof(M));
  }

  /**
   * @brief Assign operator
   */
  Matrix2& operator=(const Matrix2 &src){
    matrixDbgPrint("Matrix assign operator\n");

    if (this == &src)
      return *this;

    /* */
    memcpy(this->M, src.M, sizeof(M));
    return *this;
  }

  /**
   * @note    here is no need to check sizes at run time
   */
  Matrix2 operator + (const Matrix2 &S) const {
    matrixDbgPrint("Matrix + operator\n");
    Matrix2<T, r, c> ret;

    for (size_t i=0; i<(r*c); i++){
      ret.M[i] = this->M[i] + S.M[i];
    }
    return ret;
  }

  /**
   * @note    here is no need to check sizes at run time
   */
  Matrix2 operator - (const Matrix2 &S) const {
    matrixDbgPrint("Matrix - operator\n");
    Matrix2<T, r, c> ret;

    for (size_t i=0; i<(r*c); i++){
      ret.M[i] = this->M[i] - S.M[i];
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
   * Invers matrix itself
   * The function returns 1 on success, 0 on failure.
   */
  int32_t inverse(void){
    matrixDbgCheck(this->col == this->row, "matrix must be square");
    return matrix_inverse(static_cast<int32_t>(this->col), this->m);
  }

  /**
   * @brief operator !
   * @return inverse matrix
   */
  //X = !M
  Matrix2 operator !(void){
    Matrix2<T, r, c> ret(*this);
    int res;
    res = matrix_inverse(r,ret.M);
    matrixDbgCheck(res == 1, "Inverse failure");//Insert analizer here
    return ret;
  }
//  template <typename T, int r, int c>
//  Matrix2<T, c, r> operator~ (const Matrix2<T, r, c> &right){
//    Matrix2<T, c, r> ret;
//    matrix_transpose(r, c, right.M, ret.M);
//    return ret;
//  }

  /**
   * @note    here is no need to check sizes at run time
   */
  void operator += (const Matrix2 &S){
    matrixDbgPrint("Matrix += operator\n");
    for (size_t i=0; i<(r*c); i++){
      M[i] += S.M[i];
    }
  }

  void operator -= (const Matrix2 &S){
    matrixDbgPrint("Matrix += operator\n");
    for (size_t i=0; i<(r*c); i++){
      M[i] -= S.M[i];
    }
  }
  /**
   * @brief Subindex for Matrix elements assignation.
   * @param r
   * @param c
   * @return pointer to the element.
   */
  T& operator() (const size_t row, const size_t col){
    return this->M[calc_subindex(row,col)];
  }

  /**
   * @brief Subindex for Matrix element.
   * @param r
   * @param c
   * @return the element.
   */
  T operator() (size_t row, size_t col) const{
    return this->M[calc_subindex(row, col)];
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
        matrixDbgPrint(M[i*r + j]);
        matrixDbgPrint(" ");
      }
      matrixDbgPrint("\n");
    }
  }

private:
  /**
   * @brief calculate subindex from row and col values
   */
  size_t calc_subindex(size_t row, size_t col) const {
    matrixDbgCheck((c > col) && (r > row), "overflow");
    return row*c + col;
  }


};


/**
 * Transpose matrix
 */
template <typename T, int r, int c>
Matrix2<T, c, r> operator~ (const Matrix2<T, r, c> &right){
  Matrix2<T, c, r> ret;
  matrix_transpose(r, c, right.M, ret.M);
  return ret;
}


template <typename T, int m, int n, int p>
Matrix2<T, m, p> operator * (const Matrix2<T, m, n> &left, const Matrix2<T, n, p> &right) {
  matrixDbgPrint("Matrix * operator\n");
  Matrix2<T, m, p> ret;
  matrix_multiply<T>(m, n, p, left.M, right.M, ret.M);
  return ret;
}

template <typename T, int m, int n, int p>
Matrix2<T, m, p> nrvo_mul(const Matrix2<T, m, n> &left, const Matrix2<T, n, p> &right) {
  Matrix2<T, m, p> ret(left, right);
  return ret;
}

//template <typename T, int r, int c, int rnew>
//Matrix2<T, rnew, c> submatrix(Matrix2<T, r, c> &left) {
//  Matrix2<T, rnew, c> ret;
//  ret.m = left.m + r;
//  return ret;
//}

#endif // MATRIX2_H















}// matrix namespace
