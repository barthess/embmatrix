#ifndef VECTOR_NEW_HPP_
#define VECTOR_NEW_HPP_

#include "base.hpp"

/**
 * Convenient class
 * 1-d matrix
 */
template<typename T, int N>
class Vector : public Matrix<T>{
protected:
  T m[N];

public:
  Vector(void) :
    Matrix<T>(m, N, 1, sizeof(m))
  {
    for (uint32_t i=0; i<N; i++)
      m[i] = 0;
  }

  Vector(const T *initvector) :
    Matrix<T>(m, N, 1, sizeof(m))
  {
    for (uint32_t i=0; i<N; i++)
      m[i] = initvector[i];
  }

  Vector(T pattern) :
    Matrix<T>(m, N, 1, sizeof(m))
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
    Matrix<T>::operator =(src);
    return *this;
  }
};

#endif /* VECTOR_NEW_HPP_ */
