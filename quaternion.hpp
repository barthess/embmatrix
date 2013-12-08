#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_

#include "vector.hpp"
#include "vector3d.hpp"
namespace matrix {

/**
 *
 */
template <typename T>
void Quat2Euler(const T *q, T *e){
  T Rlb23, Rlb22, Rlb31, Rlb11, Rlb21;

  Rlb23 = 2 * (q[2] * q[3] - q[0] * q[1]);
  Rlb22 = q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
  Rlb31 = 2 * (q[1] * q[3] - q[0] * q[2]);
  Rlb11 = q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
  Rlb21 = 2 * (q[0] * q[3] + q[1] * q[2]);

  e[0] = -atan2(Rlb23, Rlb22);   //gamma крен
  e[1] = -atan2(Rlb31, Rlb11);   //psi рыскание
  e[2] =  asin(Rlb21);          //theta тангаж
}

/**
 * Quaternion.
 *
 */
template<typename T>
class Quaternion : public Vector<T, 4>{

public:
  Quaternion(void) :
    Vector<T, 4>()
  {
    this->m[0] = 1;
    this->m[1] = 0;
    this->m[2] = 0;
    this->m[3] = 0;
  }

  Quaternion(T q0, T q1, T q2, T q3) :
    Vector<T, 4>()
  {
    this->m[0] = q0;
    this->m[1] = q1;
    this->m[2] = q2;
    this->m[3] = q3;
  }

  Quaternion(const T *initvector) :
    Vector<T, 4>(initvector)
  {
  }

  Quaternion(T pattern) :
    Vector<T, 4>(pattern)
  {
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  Quaternion(const Quaternion &q);

  /**
   * @brief   complex conjugate. Modify data inplace
   */
  void ccon(void){
    this->m[1] = -this->m[1];
    this->m[2] = -this->m[2];
    this->m[3] = -this->m[3];
  }

  /**
   * @brief   complex conjugate.
   */
  void ccon(Quaternion *result) const {
    result->m[0] =  this->m[0];
    result->m[1] = -this->m[1];
    result->m[2] = -this->m[2];
    result->m[3] = -this->m[3];
  }

  /**
   * @brief   Convert quaternion to euler angles.
   */
  void euler(Vector<T, 3> *e) const {
    Quat2Euler(this->m, e->getArray());
  }

  /**
   * @brief Quaternion multiplications
   */
  void mul(const Quaternion<T> *right, Quaternion<T> *result) const {
    matrixDbgCheck(((this != right) && (this != result) && (result != right)),
        "this function can not work inplace");

    const T *r = right->m;
    const T *m = this->m;
    T *res = result->m;

    res[0] = m[0]*r[0] - m[1]*r[1] - m[2]*r[2] - m[3]*r[3];
    res[1] = m[1]*r[0] + m[0]*r[1] - m[3]*r[2] + m[2]*r[3];
    res[2] = m[2]*r[0] + m[3]*r[1] + m[0]*r[2] - m[1]*r[3];
    res[3] = m[3]*r[0] - m[2]*r[1] + m[1]*r[2] + m[0]*r[3];
  }

  /**
   * @brief   Copy operator.
   */
  Quaternion& operator=(const Quaternion &src){
    Vector<T, 4>::operator =(src);
    return *this;
  }
};

/**
 *
 */
template<typename T>
Quaternion<T> Qmul(const Quaternion<T> *left, const Quaternion<T> *right) {
  Quaternion<T> res;
  left->mul(right, &res);
  return res;
}

/**
 *
 */
template <typename T>
void __euler2quat(T *q, const T *eu){
  T phi05, theta05, psi05;
  T q_in[4];
  T cph05, sph05, cth05, sth05, cp05, sp05;
  T iqn;
  phi05   = eu[0]/2;
  theta05 = eu[1]/2;
  psi05   = eu[2]/2;
  cph05   = cos(phi05);
  sph05   = sin(phi05);
  cth05   = cos(theta05);
  sth05   = sin(theta05);
  cp05    = cos(psi05);
  sp05    = sin(psi05);
  q_in[0] = cph05*cth05*cp05 + sph05*sth05*sp05;
  q_in[1] = sph05*cth05*cp05 - cph05*sth05*sp05;
  q_in[2] = cph05*sth05*cp05 + sph05*cth05*sp05;
  q_in[3] = cph05*cth05*sp05 - sph05*sth05*cp05;

  iqn = 1/(q_in[0]*q_in[0] + q_in[1]*q_in[1] + q_in[2]*q_in[2] + q_in[3]*q_in[3]);

  // normalize and negate if q0<0
  for (size_t i=0; i<=3; i++) {
    q_in[i] = q_in[i] * iqn;
    if (q_in[0] < 0)
      q_in[i] = -q_in[i];
  }

  q[0] = q_in[0];
  q[1] = q_in[1];
  q[2] = q_in[2];
  q[3] = q_in[3];
}

/**
 *
 */
template <typename T>
void mEuler2Quat(Quaternion<T> *q, const Vector3d<T> *eu){
  T *eu_int,*q_int;
  eu_int = eu->getArray();
  q_int = q->getArray();
  __euler2quat<T>(q_int,eu_int);
}

/**
 *
 */
template<typename T>
Quaternion<T> Qcon(const Quaternion<T> *q){
  Quaternion<T> res;
  q->ccon(&res);
  return res;
}

} //namespace matrix
#endif /* QUATERNION_HPP_ */
