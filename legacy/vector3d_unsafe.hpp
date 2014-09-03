#ifndef VECTOR3D_UNSAFE_HPP_
#define VECTOR3D_UNSAFE_HPP_

#include "vector_unsafe.hpp"
namespace matrix {

/**
 * Vector3d.
 * Convenience class. 3 component vector.
 */
template<typename T>
class Vector3dUnsafe : public VectorUnsafe<T, 3>{

public:
  Vector3dUnsafe(void) :
    VectorUnsafe<T, 3>()
  {
  }

  Vector3dUnsafe(T *m) :
    VectorUnsafe<T, 3>(m, 3*sizeof(T))
  {
  }

  Vector3dUnsafe(const T *initvector) :
    VectorUnsafe<T, 3>(initvector)
  {
  }

  Vector3dUnsafe(const T pattern) :
    VectorUnsafe<T, 3>(pattern)
  {
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  Vector3dUnsafe(const Vector3dUnsafe &v);

  /**
   * @brief   Copy operator.
   */
  Vector3dUnsafe& operator=(const Vector3dUnsafe &src){
    VectorUnsafe<T, 3>::operator=(src);
    return *this;
  }
};

/**
 *
 */
template <typename T>
T dot(const Vector3dUnsafe<T> &left, const Vector3dUnsafe<T> &right){
  T *a = left->m;
  T *b = right->m;
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/**
 * cross product in right handed 3-d space
 */
template <typename T>
Vector3dUnsafe<T> cross(const Vector3dUnsafe<T> &left, const Vector3dUnsafe<T> &right){
  T res[3];
  Vector3dUnsafe<T> result(res);
  T *b   = right.getArray();
  T *m   = left.getArray();

  res[0] = m[1]*b[2] - m[2]*b[1];
  res[1] = m[2]*b[0] - m[0]*b[2];
  res[2] = m[0]*b[1] - m[1]*b[0];

  return result;
}

} // namespace matrix
#endif /* VECTOR3D_UNSAFE_HPP_ */
