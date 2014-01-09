#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_

#include "vector.hpp"
#include "vector3d.hpp"
#include "matrix2.hpp"
namespace matrix {

/**
 *
 */

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
static void __euler2quat(T *q, const T *eu){

  T phi05, theta05, psi05;
  T q_in[4];
  T cph05, sph05, cth05, sth05, cp05, sp05;
  T iqn;

  phi05   = eu[0] / 2;
  theta05 = eu[1] / 2;
  psi05   = eu[2] / 2;
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

  iqn = 1 / matrix_modulus(q_in, 4);

  // normalize and negate if q0<0
  for (size_t i=0; i<=3; i++) {
    q_in[i] = q_in[i] * iqn;
    if (q_in[0] < 0)
      q[i] = -q_in[i];
    else{
      q[i] = q_in[i];
    }
  }
}

template <typename T>
static void __euler2dcm(T *Cnb,const T *eu){
  T phi, theta, psi;
  T cph, sph;
  T cth, sth;
  T cp,  sp;

  phi   = eu[0];   // roll
  theta = eu[1];   // pitch
  psi   = eu[2];   // yaw

  cph = cos(phi);
  sph = sin(phi);
  cth = cos(theta);
  sth = sin(theta);
  cp  = cos(psi);
  sp  = sin(psi);

  Cnb[0] = cp*cth;
  Cnb[1] = cp*sph*sth - cph*sp;
  Cnb[2] = sph*sp + cph*cp*sth;
  Cnb[3] = cth*sp;
  Cnb[4] = cph*cp + sph*sp*sth;
  Cnb[5] = cph*sp*sth - cp*sph;
  Cnb[6] = -sth;
  Cnb[7] = cth*sph;
  Cnb[8] = cph*cth;
}

template <typename T>
static void __dcm2euler(T *eu,const T *Cnb){
  T Cnb11, Cnb21, Cnb31, Cnb32, Cnb33;
  T phi, theta, psi;
  const T c2pi = 6.283185307179586;
  Cnb11 = Cnb[0];
  Cnb21 = Cnb[3];
  Cnb31 = Cnb[6];
  Cnb32 = Cnb[7];
  Cnb33 = Cnb[8];

  phi   = atan2( Cnb32, Cnb33);                         // roll
  theta = atan2(-Cnb31, sqrt(Cnb32*Cnb32+Cnb33*Cnb33)); // pitch
  psi   = atan2( Cnb21, Cnb11);                         // yaw

  if (psi < 0)
    psi = psi+c2pi;

  eu[0] = phi;
  eu[1] = theta;
  eu[2] = psi;
}


template <typename T>
static void __quat2euler(T *e,const T *q){
  T Cnb31, Cnb32, Cnb33, Cnb21, Cnb11;
  T q1, q2, q3, q4;
  const T c2pi = 6.283185307179586;
  T eu_int[3];

  q1 = q[0];
  q2 = q[1];
  q3 = q[2];
  q4 = q[3];

  Cnb11 = q1*q1 + q2*q2 - q3*q3 - q4*q4;
  Cnb21 = 2 * (q2*q3 + q1*q4);
  Cnb31 = 2 * (q2*q4 - q1*q3);
  Cnb32 = 2 * (q3*q4 + q1*q2);
  Cnb33 = q1*q1 - q2*q2 - q3*q3 + q4*q4;

  eu_int[0] = atan2(Cnb32,  Cnb33);                               //roll крен
  eu_int[1] = atan2(-Cnb31, sqrt(Cnb32*Cnb32 + Cnb33*Cnb33));     //theta тангаж
  eu_int[2] = atan2(Cnb21,  Cnb11);                               //psi курс

  /* wrap 2pi */
  if (eu_int[2] < 0){
    eu_int[2] = eu_int[2] + c2pi;
  }

  e[0] = eu_int[0];
  e[1] = eu_int[1];
  e[2] = eu_int[2];
}
/**
 *
 */
template <typename T>
void Euler2Quat(Quaternion<T> *q, const Vector3d<T> *eu){
  T *eu_int, *q_int;
  eu_int = eu->getArray();
  q_int = q->getArray();
  __euler2quat<T>(q_int, eu_int);
}

template <typename T>
void Quat2Euler(Vector3d<T> *eu,const Quaternion<T> *q){
  T *eu_int, *q_int;
  eu_int = eu->getArray();
  q_int = q->getArray();
  __quat2euler<T>(eu_int, q_int);
}

template <typename T>
void Euler2DCM(MatrixUnsafe<T> *Cnb, const Vector3d<T> *eu){
  T *eu_int, *Cnb_int;
  eu_int = eu->getArray();
  Cnb_int = Cnb->getArray();
  __euler2dcm<T>(Cnb_int, eu_int);
}

template <typename T>
void DCM2Euler(Vector3d<T> *eu, MatrixUnsafe<T> *Cnb){
  T *eu_int, *Cnb_int;
  eu_int = eu->getArray();
  Cnb_int = Cnb->getArray();
  __dcm2euler<T>(eu_int, Cnb_int);
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

/**
 * Quaternionian vector rotation
 */
template <typename T>
void QuatRot(Vector3d<T> *v_rot, const Quaternion<T> *q, const Vector3d<T> *v_in){
  Quaternion<T> qV, qVr, qc, tmp;

  qV(0) = static_cast<T>(0);
  qV(1) = (*v_in)(0);
  qV(2) = (*v_in)(1);
  qV(3) = (*v_in)(2);

  qc  = Qcon(q);
  tmp = Qmul(q, &qV);
  qVr = Qmul(&tmp, &qc);

  (*v_rot)(0) = qVr(1);
  (*v_rot)(1) = qVr(2);
  (*v_rot)(2) = qVr(3);
}

} //namespace matrix
#endif /* QUATERNION_HPP_ */
