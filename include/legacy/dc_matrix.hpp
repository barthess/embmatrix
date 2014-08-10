/*
 * direction cosine matrix
 */

/*
 *       | Rxx Rxy Rxz |
 * DCM = | Ryx Ryy Ryz |
 *       | Rzx Rzy Rzz |
 */

#ifndef DC_MATRIX_HPP_
#define DC_MATRIX_HPP_

#include "matrix.hpp"
#include "vector3d_unsafe.hpp"
namespace matrix {

/**
 *
 */
template <typename T>
class DCMatrix : public Matrix<T, 3, 3> {
public:
  DCMatrix(void) :
    x(&this->m[0]),
    y(&this->m[3]),
    z(&this->m[6])
  {
    this->m[0] = 1; this->m[1] = 0; this->m[2] = 0;
    this->m[3] = 0; this->m[4] = 1; this->m[5] = 0;
    this->m[6] = 0; this->m[7] = 0; this->m[8] = 1;
  }

  Vector3dUnsafe<T> &Rx(void) {return x;}
  Vector3dUnsafe<T> &Ry(void) {return y;}
  Vector3dUnsafe<T> &Rz(void) {return z;}

private:
  Vector3dUnsafe<T> x;
  Vector3dUnsafe<T> y;
  Vector3dUnsafe<T> z;
};

} //namespace matrix

#endif /* DC_MATRIX_HPP_ */
