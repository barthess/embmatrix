#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "matrix.hpp"

/**
 * Convenient class
 * 1-d matrix
 */
template<typename T, int N>
class Vector : public Matrix<T, 1, N>{
protected:
  T m[N];

public:
  Vector(void) :
    Matrix<T, 1, N>(m, 1, N, sizeof(m))
  {
    for (uint32_t i=0; i<N; i++)
      m[i] = 0;
  }

  Vector(const T *initvector) :
    Matrix<T, 1, N>(m, 1, N, sizeof(m))
  {
    for (uint32_t i=0; i<N; i++)
      m[i] = initvector[i];
  }

  Vector(T pattern) :
    Matrix<T, 1, N>(m, 1, N, sizeof(m))
  {
    for (uint32_t i=0; i<N; i++)
      m[i] = pattern;
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
    matrixDbgCheck(v < sizeof(m)/sizeof(m[0]), "overflow");
    return this->m[v];
  }

  /**
   * @brief Subindex for Matrix element. Single dimmension variant.
   * @param v
   * @return the element.
   */
  T operator() (const uint32_t v) const{
    matrixDbgCheck(v < sizeof(m)/sizeof(m[0]), "overflow");
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

#endif /* VECTOR_HPP_ */
