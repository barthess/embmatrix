#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include "vector.hpp"
namespace matrix {

/**
 * Vector3d.
 * Convenience class. 3 component vector.
 */
template<typename T>
class Vector3d : public Vector<T, 3>{

public:
  Vector3d(void) :
    Vector<T, 3>()
  {
  }

  Vector3d(T m0, T m1, T m2) :
    Vector<T, 3>()
  {
    this->m[0] = m0;
    this->m[1] = m1;
    this->m[2] = m2;
  }

  Vector3d(const T *initvector) :
    Vector<T, 3>(initvector)
  {
  }

  Vector3d(const T pattern) :
    Vector<T, 3>(pattern)
  {
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  Vector3d(const Vector3d &v);

  /**
   * Dot product of 2 vectors
   */
  T dot(Vector3d<T> *right){
    T *a = this->m;
    T *b = right->m;
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  }

  void norm(void){
    matrix_normalize(this->m, 3);
  }

  /**
   * cross product in right handed 3-d space
   */
  void cross(Vector3d<T> *right, Vector3d<T> *result){
    T *b = right->m;
    T *res = result->m;
    T *m = this->m;

    matrixDbgCheck(((b != res) && (b != m) && (res != m)),
        "this functions can not work inplace");

    res[0] = m[1]*b[2] - m[2]*b[1];
    res[1] = m[2]*b[0] - m[0]*b[2];
    res[2] = m[0]*b[1] - m[1]*b[0];
  }

  /**
   * @brief   Copy operator.
   */
  Vector3d& operator=(const Vector3d &src){
    Vector<T, 3>::operator=(src);
    return *this;
  }
};

/**
 *
 */
template <typename T>
T dot(const Vector3d<T> &left, const Vector3d<T> &right){
  T *a = left->m;
  T *b = right->m;
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/**
 * cross product in right handed 3-d space
 */
template <typename T>
Vector3d<T> cross(const Vector3d<T> &left, const Vector3d<T> &right){
  Vector3d<T> result;
  T *res = result->m;
  T *b   = right->m;
  T *m   = left->m;

  res[0] = m[1]*b[2] - m[2]*b[1];
  res[1] = m[2]*b[0] - m[0]*b[2];
  res[2] = m[0]*b[1] - m[1]*b[0];

  return result;
}

} // namespace matrix
#endif /* VECTOR3D_HPP_ */
