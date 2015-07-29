#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <utility> // for std::move
#include <cstring> // memcpy, memset

#include "matrix_osal.hpp"
#include "matrix_primitives.hpp"

#define MATRIX_COPY_CTOR_ENABLED 1
#define MATRIX_COPY_OPERATOR_ENABLED 1

namespace matrix {

template<typename T, size_t r, size_t c>
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
   * @brief   Copy constructor. Keep it for testing
   */
#if MATRIX_COPY_CTOR_ENABLED
  Matrix(const Matrix &src){
    matrixDbgPrint("Matrix copy constructor\n");
    _default_ctor();
    memcpy(this->M, src.M, msize());
    this->tr = src.tr;
  }
#else
  Matrix(const Matrix &src) = delete; /* Copy forbidden */
#endif /* MATRIX_COPY_CTOR_FORBIDDEN */

  /**
   * @brief Copy operator
   */
#if MATRIX_COPY_OPERATOR_ENABLED
  Matrix& operator = (const Matrix &src) {
    matrixDbgPrint("Matrix copy operator\n");
    if (this == &src) {
      return *this;
    } else {
      memcpy(this->M, src.M, msize());
      this->tr = src.tr;
      return *this;
    }
  }
#else
  void operator = (const Matrix &src) = delete; /* Assign forbidden */
#endif /* MATRIX_COPY_OPERATOR_ENABLED */

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
      M[i] = pattern;
  }

  /**
   *
   */
  Matrix(T pattern, T diag): Matrix(pattern) {
    matrixDbgPrint("Matrix diagonal constructor\n");
    size_t diagSize = (c > r)? r : c;
    for(size_t i = 0; i < diagSize; i++)
      this->M[i*c + i] = diag;
  }

  /**
   *
   */
  Matrix(const T *array, size_t arraysize) {
    matrixDbgPrint("Matrix const array constructor\n");
    matrixDbgCheck(msize() == arraysize); /* sizes mismatch */
    _default_ctor();
    memcpy(M, array, arraysize);
  }

  /**
   *
   */
  void transpose_hack(void) {
    this->tr = !this->tr;
  }

  /**
   *
   */
  bool transposed(void) const {
    return this->tr;
  }

  /**
   * @brief Move operator
   */
  Matrix& operator = (Matrix &&src) {
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
   *
   */
  void operator += (const Matrix &right) {
    matrixDbgPrint("Matrix += operator\n");

    if (!this->tr && !right.tr)
      matrix_increase(r*c, right.M, this->M);
    else if (this->tr && !right.tr)
      matrix_increase_TB(r, c, right.M, this->M);
    else if (!this->tr && right.tr)
      matrix_increase_TA(r, c, right.M, this->M);
    else
      matrix_increase(r*c, right.M, this->M);
  }

  /**
   *
   */
  void operator -= (const Matrix &right) {
    matrixDbgPrint("Matrix -= operator\n");

    if (!this->tr && !right.tr)
      matrix_decrease(r*c, right.M, this->M);
    else if (this->tr && !right.tr)
      matrix_decrease_TB(r, c, right.M, this->M);
    else if (!this->tr && right.tr)
      matrix_decrease_TA(r, c, right.M, this->M);
    else
      matrix_decrease(r*c, right.M, this->M);
  }

  /**
   *
   */
  void operator *= (T scale) {
    matrixDbgPrint("Matrix *= operator\n");
    for (size_t i=0; i<r*c; i++)
      this->M[i] *= scale;
  }

  /**
   *
   */
  void operator /= (T scale) {
    matrixDbgPrint("Matrix /= operator\n");
    scale = static_cast<T>(1) / scale;
    for (size_t i=0; i<r*c; i++)
      this->M[i] *= scale;
  }

  /**
   * @brief Subindex for Matrix elements assignation.
   * @param r
   * @param c
   * @return pointer to the element.
   */
  T& operator() (size_t row, size_t col) {
    return M[calc_subindex(row, col)];
  }

  /**
   * @brief Subindex for Matrix element.
   * @param r
   * @param c
   * @return the element.
   */
  T operator() (size_t row, size_t col) const {
    return M[calc_subindex(row, col)];
  }

  /**
   * @brief Transpose operator
   * @note  Performs array move
   */
  template <typename U, size_t m, size_t n>
  friend Matrix<U, n, m> operator ~ (Matrix<U, m, n> &&left);

  /**
   * @brief Transpose operator
   * @note  Performs deep array move
   */
  template <typename U, size_t m, size_t n>
  friend Matrix<U, n, m> transpose_deep(Matrix<U, m, n> &&left);

  /**
   * @brief Return matrix size in bytes
   */
  constexpr size_t msize(void) {
    return sizeof(T) * r * c;
  }

private:
  /**
   * @note  WARNING!!! no size checks
   * @note  Use it only if you know what to do
   * @note  Keep it for move transpose operator
   * @brief Matrix array's wrapper constructor
   */
  Matrix(T *array, bool tr) {
    matrixDbgPrint("Matrix array's wrapper constructor\n");
    this->M = array;
    this->tr = tr;
  }

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

  void _default_ctor(void) {
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
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator -(const Matrix<T, m, n> &right) {
  return right*static_cast<T>(-1.0);
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator -(Matrix<T, m, n> &&right) {
  return std::move(right)*static_cast<T>(-1.0);
}

/**
 * @note    here is no need to check sizes at run time
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator - (const Matrix<T, m, n> &left,
                            const Matrix<T, m, n> &right) {
  matrixDbgPrint("Matrix - operator\n");
  Matrix<T, m, n> ret;

  if (!left.tr && !right.tr)
    matrix_substract(m*n, left.M, right.M, ret.M);
  else if (left.tr && !right.tr)
    matrix_substract_TA(m, n, left.M, right.M, ret.M);
  else if (!left.tr && right.tr)
    matrix_substract_TB(m, n, left.M, right.M, ret.M);
  else
    matrix_substract_TAB(m, n, left.M, right.M, ret.M);

  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator - (Matrix<T, m, n> &&left,
                            const Matrix<T, m, n> &right) {
  matrixDbgPrint("Matrix - rvalue operator\n");
  Matrix<T, m, n> ret(std::move(left));
  ret -= right;
  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator - (const Matrix<T, m, n> &left,
                            Matrix<T, m, n> &&right) {
  matrixDbgPrint("Matrix - rvalue operator\n");
  Matrix<T, m, n> ret(std::move(right));
  ret -= left;
  ret = std::move(ret)*static_cast<T>(-1.0);
  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator - (Matrix<T, m, n> &&left,
                            Matrix<T, m, n> &&right) {
  matrixDbgPrint("Matrix - rvalue operator\n");
  Matrix<T, m, n> ret(std::move(left));
  ret -= right;
  return ret;
}

/**
 * @note    here is no need to check sizes at run time
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator + (const Matrix<T, m, n> &left,
                            const Matrix<T, m, n> &right) {
  matrixDbgPrint("Matrix + operator\n");
  Matrix<T, m, n> ret;

  if (!left.tr && !right.tr)
    matrix_add(m*n, left.M, right.M, ret.M);
  else if (left.tr && !right.tr)
    matrix_add_TA(m, n, left.M, right.M, ret.M);
  else if (!left.tr && right.tr)
    matrix_add_TB(m, n, left.M, right.M, ret.M);
  else
    matrix_add_TAB(m, n, left.M, right.M, ret.M);

  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator + (const Matrix<T, m, n> &left,
                            Matrix<T, m, n> &&right) {
  matrixDbgPrint("Matrix + rvalue operator\n");
  Matrix<T, m, n> ret(std::move(right));
  ret += left;
  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator + (Matrix<T, m, n> &&left,
                            const Matrix<T, m, n> &right) {
  matrixDbgPrint("Matrix + rvalue operator\n");
  Matrix<T, m, n> ret(std::move(left));
  ret += right;
  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator + (Matrix<T, m, n> &&left,
                            Matrix<T, m, n> &&right) {
  matrixDbgPrint("Matrix + rvalue operator\n");
  Matrix<T, m, n> ret(std::move(left));
  ret += right;
  return ret;
}

/**
 * Multiplication operator
 */
template <typename T, size_t m, size_t n, size_t p>
Matrix<T, m, p> operator * (const Matrix<T, m, n> &left,
                            const Matrix<T, n, p> &right) {
  Matrix<T, m, p> ret;
  matrixDbgPrint("Matrix multiply operator\n");

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
 * Scale operator
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator * (const Matrix<T, m, n> &left, T scale) {
  Matrix<T, m, n> ret;
  matrixDbgPrint("Matrix scale operator\n");
  matrix_scale(ret.M, left.M, scale, m*n);
  return ret;
}

/**
 * Overloaded scale operator (r-value)
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator * (Matrix<T, m, n> &&left, T scale) {
  Matrix<T, m, n> ret(std::move(left));
  matrixDbgPrint("Matrix scale operator\n");
  for (size_t i = 0; i < m*n; i++)
    ret.M[i] *= scale;
  return ret;
}

/**
 * Multiply operator returning scalar type
 */
template <typename T, size_t n>
T operator * (const Matrix<T, 1, n> &left, const Matrix<T, n, 1> &right) {
  matrixDbgPrint("Matrix vector mul operator\n");
  return vector_multiply(left.M, right.M, n);
}

/**
 * Overloaded scale operator for reverse operand order
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator * (T scale, const Matrix<T, m, n> &right) {
  return right * scale;
}

/**
 * Overloaded scale operator for reverse operand order (r-value)
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator * (T scale, Matrix<T, m, n> &&right) {
  return std::move(right) * scale;
}

/**
 * Scale (division) operator
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator / (const Matrix<T, m, n> &left, T scale) {
  return left * (static_cast<T>(1) / scale);
}

/**
 * Overloaded scale (division) operator (r-value)
 */
template <typename T, size_t m, size_t n>
Matrix<T, m, n> operator / (Matrix<T, m, n> &&left, T scale) {
  return std::move(left) * (static_cast<T>(1.0) / scale);
}

/**
 * @brief Inverse operator
 */
template <typename T, size_t m>
Matrix<T, m, m> operator ! (const Matrix<T, m, m> &left) {
  matrixDbgPrint("Matrix inverse operator\n");
  Matrix<T, m, m> ret(left);
  if (ret.tr) {
    ret = transpose_deep(std::move(ret));
    ret.tr = false;
  }
  /* The function returns 1 on success, 0 on failure. */
  int inv_res = matrix_inverse(m, ret.M);
  matrixDbgCheck(1 == inv_res); /* matrix inversion failed */
  return ret;
}

/**
 * @brief Inverse operator
 */
template <typename T, size_t m>
Matrix<T, m, m> operator ! (Matrix<T, m, m> &&left) {
  matrixDbgPrint("Matrix move inverse operator\n");
  Matrix<T, m, m> ret(std::move(left));
  if (ret.tr) {
    ret = transpose_deep(std::move(ret));
    ret.tr = false;
  }
  /* The function returns 1 on success, 0 on failure. */
  int inv_res = matrix_inverse(m, ret.M);
  matrixDbgCheck(1 == inv_res); /* matrix inversion failed */
  return ret;
}

/**
 * @brief Transpose operator
 * @note  Performs array copy
 */
template <typename T, size_t m, size_t n>
Matrix<T, n, m> operator ~ (const Matrix<T, m, n> &left) {
  matrixDbgPrint("Matrix copy transpose\n");
  Matrix<T, n, m> ret;
  ret.tr = !left.tr;
  memcpy(ret.M, left.M, ret.msize());
  return ret;
}

/**
 * @brief Transpose operator
 * @note  Performs deep array copy
 */
template <typename T, size_t m, size_t n>
Matrix<T, n, m> transpose_deep(const Matrix<T, m, n> &left) {
  matrixDbgPrint("Matrix deep transpose\n");
  Matrix<T, n, m> ret;

  matrix_deep_transpose(m, n, left.M, ret.M);
  ret.tr = false; /* this functions always returns direct matrices */
  return ret;
}

/**
 * @brief Transpose operator
 * @note  Performs array move
 */
template <typename U, size_t m, size_t n>
Matrix<U, n, m> operator ~ (Matrix<U, m, n> &&left) {
  matrixDbgPrint("Matrix move transpose\n");
  Matrix<U, n, m> ret(left.M, left.tr);
  left.M = nullptr;
  ret.tr = !ret.tr;
  return ret;
}

/**
 * @brief Transpose operator
 * @note  Performs deep array move
 */
template <typename U, size_t m, size_t n>
Matrix<U, n, m> transpose_deep(Matrix<U, m, n> &&left) {
  matrixDbgPrint("Matrix deep transpose move\n");
  /* this functions always returns direct matrices */
  Matrix<U, n, m> ret(left.M, false);
  left.M = nullptr;
  U tmp;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < m; j++) {
      tmp = ret.M[i*n + j];
      ret.M[i*n + j] = ret.M[j*n + i];
      ret.M[j*n + i] = tmp;
    }
  }
  return ret;
}

/**
 *
 */
template <typename T, size_t m, size_t n, size_t p, size_t q>
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
template <typename T, size_t r, size_t c>
Matrix<T, 1, c> row(const Matrix<T, r, c> &donor, size_t row) {
  matrixDbgCheck(row < c);

  Matrix<T, 1, c> ret;
  T *M = &donor.M[row*c];
  memcpy(ret.M, M, sizeof(T) * c);
  return ret;
}

/**
 * @brief Matrix norm
 */
template <typename T, size_t r, size_t c>
T norm(const Matrix<T, r, c> &mtrx) {
  return matrix_modulus(mtrx.M, r*c);
}

/**
 * @brief Matrix square root
 */
template <typename T, size_t size>
Matrix<T, size, size> sqrtm(const Matrix<T, size, size> &mtrx,
                            const size_t maxIter = 50,
                            const T epsilon = 1e-5) {
  matrixDbgPrint("Matrix square root\n");

  Matrix<T, size, size> tmp(0, 1);
  Matrix<T, size, size> result(mtrx), currentSqrtm, copyResult, copyTmp;
  T normDelta;

  for (size_t i = 0; i < maxIter; i++) {
    copyResult = result;
    copyTmp = tmp;
    currentSqrtm = (result + !copyTmp) * static_cast<T>(0.5);
    tmp = (tmp + !copyResult) * static_cast<T>(0.5);
    normDelta = matrix_modulus((result - currentSqrtm).M, size*size);
    result = currentSqrtm;
    if (normDelta < epsilon)
      return result;
  }
  return result;
}

} /* namespace matrix */
#endif /* MATRIX_HPP_ */
