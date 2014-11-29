#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <utility> // for std::move
#include <cstring> // memcpy, memset

#include "matrix_osal.hpp"
#include "matrix_primitives.hpp"

namespace matrix {

template<typename T, int r, int c>
class Matrix {

public:
  T *M = nullptr;
  bool tr = false; /* transpose flag */

public:
  ~Matrix(void) {
    matrix_free(pool_index(), M);
  }

  /**
   *
   */
  Matrix(void) {
    matrixDbgPrint("Matrix default constructor\n");
    _default_ctor();
  }

  /**
   * @brief   Copy constructor. Keeps here commented out only for testing
   */
//  Matrix(const Matrix3 &src){
//    matrixDbgPrint("Matrix copy constructor\n");
//    _default_ctor();
//    memcpy(this->M, src.M, msize());
//  }

  /**
   * @brief   Coping forbidden
   */
  Matrix(const Matrix&) = delete;

  /**
   * @brief   Assign forbidden
   */
  void operator=(const Matrix&) = delete;

  /**
   *
   */
  Matrix(Matrix &&src) {
    matrixDbgPrint("Matrix move constructor\n");
    tr = src.tr;
    M = src.M;
    src.M = nullptr;
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
  Matrix(const T *array, const size_t arraysize) {
    matrixDbgPrint("Matrix const array constructor\n");
    matrixDbgCheck(msize() == arraysize); /* sizes mismatch */
    _default_ctor();
    memcpy(this->M, array, arraysize);
  }

  /**
   * @brief Move operator
   */
  Matrix& operator=(Matrix &&src) {
    matrixDbgPrint("Matrix move operator\n");

    if (this == &src)
      return *this;
    else{
      matrix_free(pool_index(), M);
      tr = src.tr;
      M = src.M;
      src.M = nullptr;
      return *this;
    }
  }

  /**
   * @brief Inverse operator
   */
  Matrix& operator ! (void){
    matrixDbgPrint("Matrix inverse operator\n");
    static_assert(c == r, "matrix must be square");

    /* The function returns 1 on success, 0 on failure. */
    int inv_res = matrix_inverse(c, M);
    matrixDbgCheck(1 == inv_res); /* matrix inversion failed */
    return *this;
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

  /**
   * @note    here is no need to check sizes at run time
   */
  Matrix operator - (const Matrix &S) const {
    matrixDbgPrint("Matrix - operator\n");
    Matrix<T, r, c> ret;

    for (size_t i=0; i<(r*c); i++){
      ret.M[i] = this->M[i] - S.M[i];
    }
    return ret;
  }

  /**
   * @brief Subindex for Matrix elements assignation.
   * @param r
   * @param c
   * @return pointer to the element.
   */
  T& operator() (const size_t row, const size_t col) {
    return this->M[calc_subindex(row,col)];
  }

  /**
   * @brief Subindex for Matrix element.
   * @param r
   * @param c
   * @return the element.
   */
  T operator() (size_t row, size_t col) const {
    return this->M[calc_subindex(row, col)];
  }

  /**
   * @brief Return matrix size in bytes
   */
  constexpr size_t msize(void) {
    return sizeof(T) * r * c;
  }

private:
  /**
   * @brief Determine closest suitable pool size
   */
  constexpr size_t pool_index(void) {
    size_t firstone = sizeof(size_t) * 8 - 1;
    size_t size = msize();

    if (0 == size)
      return 0;

    while(firstone-- > 0){
      if ((size & (1 << firstone)) > 0)
        break;
    }

    if (0 == firstone)
      return 0;

    if ((size & ((1 << firstone) - 1)) == 0)
      return firstone - 4;
    else
      return firstone - 3;
  }

  void _default_ctor(void){
    static_assert((c>0) && (r>0), "Zero size forbidden");
    //this->M = static_cast<T *>(matrix_malloc(msize()));
    this->M = static_cast<T *>(matrix_malloc(pool_index(), msize()));
  }

  /**
   * @brief calculate subindex from row and col values
   */
  size_t calc_subindex(size_t row, size_t col) const {
    matrixDbgCheck((c > col) && (r > row)); /* overflow */
    return row*c + col;
  }
};

/**
 * Multiply operator
 */
template <typename T, int m, int n, int p>
Matrix<T, m, p> operator * (const Matrix<T, m, n> &left,
                            const Matrix<T, n, p> &right) {
  Matrix<T, m, p> ret;

  if (!left.tr && !right.tr)
    matrix_multiply(m, n, p, left.M, right.M, ret.M);
  else if (left.tr && !right.tr)
    matrix_multiply_TA(m, n, p, left.M, right.M, ret.M);
  else if (!left.tr && right.tr)
    matrix_multiply_TB(m, n, p, left.M, right.M, ret.M);
  else
    matrix_multiply_TAB(m, n, p, left.M, right.M, ret.M);

  return ret;
}

/**
 * Multiply operator returning scalar type
 */
template <typename T, int n>
T operator * (const Matrix<T, 1, n> &left, const Matrix<T, n, 1> &right) {
  return vector_multiply(left.M, right.M, n);
}

/**
 * @brief Transpose operator
 * @note  Performs full array copy
 */
template <typename T, int m, int n>
Matrix<T, n, m> operator ~ (const Matrix<T, m, n> &left) {
  matrixDbgPrint("Matrix copy transpose\n");
  Matrix<T, n, m> ret;
  ret.tr = !left.tr;
  memcpy(ret.M, left.M, ret.msize());
  return ret;
}

/**
 * @brief Transpose operator
 * @note  Uses move semantic without coping
 */
template <typename T, int m, int n>
Matrix<T, n, m> operator ~ (Matrix<T, m, n> &&left) {
  matrixDbgPrint("Matrix move transpose\n");
  Matrix<T, n, m> ret(std::move(left));
  ret.tr = !ret.tr;
  return ret;
}

/**
 *
 */
template <typename T, int m, int n, int p, int q>
void patch(Matrix<T, m, n> &acceptor, const Matrix<T, p, q> &patch,
                                                size_t row, size_t col) {
  matrixDbgCheck((row + p) <= m && (col + q) <= n);
  T *ap, *pp;
  for (size_t i = 0; i<p; i++){
    ap = &acceptor.M[n*(row+i) + col];
    pp = &patch.M[i*q];
    memcpy(ap, pp, sizeof(T)*q);
  }
}

/**
 *
 */
template <typename T, int r, int c>
Matrix<T, 1, c> row(const Matrix<T, r, c> &donor, size_t row) {
  matrixDbgCheck(row < c);

  Matrix<T, 1, c> ret;
  T *M = &donor.M[row*c];
  memcpy(ret.M, M, sizeof(T) * c);
  return ret;
}

} /* namespace matrix */
#endif /* MATRIX_HPP_ */
