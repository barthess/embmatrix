#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "matrix.hpp"
namespace matrix {

/**
 * Convenient class
 * 1-d matrix
 */
template<typename T, int N>
class Vector : public Matrix<T, N, 1>{
public:
  Vector(void) :
    Matrix<T, N, 1>()
  {
  }

  Vector(const T *initvector) :
    Matrix<T, N, 1>(initvector)
  {
  }

  Vector(T pattern) :
    Matrix<T, N, 1>(pattern)
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
    Matrix<T, N, 1>::operator =(src);
    return *this;
  }
};

}// namespace matrix
#endif /* VECTOR_HPP_ */
