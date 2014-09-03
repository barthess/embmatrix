#ifndef VECTOR_UNSAFE_HPP_
#define VECTOR_UNSAFE_HPP_

#include "matrix_unsafe.hpp"
namespace matrix {

/**
 * Convenient class
 * 1-d matrix
 */
template<typename T, int N>
class VectorUnsafe : public MatrixUnsafe<T>{
public:
  VectorUnsafe(T *m, size_t bufsize) :
    MatrixUnsafe<T>(m, 1, N, bufsize)
  {
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  VectorUnsafe(const VectorUnsafe &v);

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
  VectorUnsafe& operator=(const VectorUnsafe &src){
    MatrixUnsafe<T>::operator =(src);
    return *this;
  }
};

} //namespace matrix
#endif /* VECTOR_UNSAFE_HPP_ */
