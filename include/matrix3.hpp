#ifndef MATRIX3_HPP_
#define MATRIX3_HPP_

#include "matrix_platform.h"
#include "string.h"
#include "matrix_primitives.hpp"

namespace matrix {

template<typename T, int r, int c>
class Matrix3 {

public:
  T *M;

public:
  ~Matrix3(void) {
    matrix_free(M);
  }

  /**
   *
   */
  Matrix3(void) {
    matrixDbgPrint("Matrix default constructor\n");
    _default_ctor();
  }

  /**
   *
   */
//  Matrix3(const Matrix3 &src){
//    matrixDbgPrint("Matrix copy constructor\n");
//    _default_ctor();
//    memcpy(this->M, src.M, msize());
//  }



  Matrix3(const Matrix3&) = delete; //запретить копирование
  void operator=(const Matrix3&) = delete; //запретить присваивание



  /**
   *
   */
  Matrix3(Matrix3 &&src){
    matrixDbgPrint("Matrix move constructor\n");
    M = src.M;
    src.M = NULL;
  }

  /**
   *
   */
  Matrix3(T pattern) {
    matrixDbgPrint("Matrix pattern constructor\n");
    _default_ctor();
    for (size_t i=0; i<(c*r); i++)
      this->M[i] = pattern;
  }

  /**
   *
   */
  Matrix3(const T *array, size_t arraysize) {
    matrixDbgPrint("Matrix const array constructor\n");
    matrixDbgCheck(msize() == arraysize, "sizes mismatch");
    _default_ctor();
    memcpy(this->M, array, arraysize);
  }

  /**
   * @brief Move operator
   */
  Matrix3& operator=(Matrix3 &&src){
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
  Matrix3 operator + (const Matrix3 &S) const {
    matrixDbgPrint("Matrix + operator\n");
    Matrix3<T, r, c> ret;

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
Matrix3<T, m, p> operator * (const Matrix3<T, m, n> &left, const Matrix3<T, n, p> &right) {
  Matrix3<T, m, p> ret;
  matrix_multiply(m, n, p, left.M, right.M, ret.M);
  return ret;
}

} /* namespace matrix */
#endif /* MATRIX3_HPP_ */
