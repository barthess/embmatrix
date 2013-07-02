#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "matrix.hpp"
namespace matrix {

/**
 * Convenient class
 * 1-d matrix
 */
template<typename T, int N>
class Vector : public Matrix<T, 1, N>{
public:
  Vector(void) :
    Matrix<T, 1, N>()
  {
  }

  Vector(const T *initvector) :
    Matrix<T, 1, N>(initvector)
  {
  }

  Vector(T pattern) :
    Matrix<T, 1, N>(pattern)
  {
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  Vector(const Vector &v);

  /**
   * @brief Subindex for Matrix elements assignation. Single dimension variant.
   * @param v
   * @return pointer to the element.
   */
  T& operator() (const uint32_t v){
    matrixDbgCheck(v < N, "overflow");
    return this->m[v];
  }

  /**
   * @brief Subindex for Matrix element. Single dimmension variant.
   * @param v
   * @return the element.
   */
  T operator() (const uint32_t v) const{
    matrixDbgCheck(v < N, "overflow");
    return this->m[v];
  }

  /**
   * @brief   Copy operator.
   */
  Vector& operator=(const Vector &src){
    Matrix<T, 1, N>::operator =(src);
    return *this;
  }
};

}// namespace matrix
#endif /* VECTOR_HPP_ */
