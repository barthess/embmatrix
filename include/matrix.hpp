#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include "matrix_osal.h"
#include "string.h"
#include "matrix_primitives.hpp"

namespace matrix {

template<typename T, int r, int c>
class Matrix {

public:
  T *M;

public:
  ~Matrix(void) {
    matrix_free(M);
  }

  /**
   *
   */
  Matrix(void) {
    matrixDbgPrint("Matrix default constructor\n");
    _default_ctor();
  }

  /**
   *
   */
//  Matrix(const Matrix3 &src){
//    matrixDbgPrint("Matrix copy constructor\n");
//    _default_ctor();
//    memcpy(this->M, src.M, msize());
//  }



  Matrix(const Matrix&) = delete; //запретить копирование
  void operator=(const Matrix&) = delete; //запретить присваивание



  /**
   *
   */
  Matrix(Matrix &&src){
    matrixDbgPrint("Matrix move constructor\n");
    M = src.M;
    src.M = NULL;
  }

  /**
   *
   */
  Matrix(T pattern) {
    matrixDbgPrint("Matrix pattern constructor\n");
    _default_ctor();
    for (size_t i=0; i<(c*r); i++)
      this->M[i] = pattern;
  }

  /**
   *
   */
  Matrix(const T *array, size_t arraysize) {
    matrixDbgPrint("Matrix const array constructor\n");
    matrixDbgCheck(msize() == arraysize, "sizes mismatch");
    _default_ctor();
    memcpy(this->M, array, arraysize);
  }

  /**
   * @brief Move operator
   */
  Matrix& operator=(Matrix &&src){
    matrixDbgPrint("Matrix move operator\n");

    if (this == &src)
      return *this;
    else{
      matrix_free(M);
      M = src.M;
      src.M = NULL;
      return *this;
    }
  }


  /**
   * @note    here is no need to check sizes at run time
   */
  Matrix operator + (const Matrix &S) const {
    matrixDbgPrint("Matrix + operator\n");
    Matrix<T, r, c> ret;

    for (size_t i=0; i<(r*c); i++){
      ret.M[i] = this->M[i] + S.M[i];
    }
    return ret;
  }




private:
  constexpr size_t msize(void){
    return sizeof(T) * r * c;
  }

  void _default_ctor(void){
    this->M = static_cast<T *>(matrix_malloc(msize()));
  }
};

/**
 *
 */
template <typename T, int m, int n, int p>
Matrix<T, m, p> operator * (const Matrix<T, m, n> &left, const Matrix<T, n, p> &right) {
  Matrix<T, m, p> ret;
  matrix_multiply(m, n, p, left.M, right.M, ret.M);
  return ret;
}

} /* namespace matrix */
#endif /* MATRIX_HPP_ */
