#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "matrix_unsafe.hpp"
namespace matrix {

/**
 * Convenient class
 * representing matrix with automatically allocated static buffer
 */
template<typename T, int r, int c>
class Matrix : public MatrixUnsafe<T>{
protected:
  T m[r*c];

public:
  Matrix(void) :
    MatrixUnsafe<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = 0;
  }

  Matrix(const T *initvector) :
    MatrixUnsafe<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = initvector[i];
  }

  Matrix(T pattern) :
    MatrixUnsafe<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = pattern;
  }

  /**
   * @brief Copy operator.
   */
  Matrix& operator=(const Matrix &src){
    MatrixUnsafe<T>::operator=(src);
    return *this;
  }
};

} // namespace matrix
#endif // MATRIX_HPP
