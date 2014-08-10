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
static void __dcm2quat(T *q, const T *Cnb){
  T Cnb11, Cnb12, Cnb13;
  T Cnb21, Cnb22, Cnb23;
  T Cnb31, Cnb32, Cnb33;
  T qnb[4];
  T trC;
  T P[4];
  T iqn;
  T mP;
  size_t sw;

  Cnb11 = Cnb[0]; Cnb12 = Cnb[1]; Cnb13 = Cnb[2];
  Cnb21 = Cnb[3]; Cnb22 = Cnb[4]; Cnb23 = Cnb[5];
  Cnb31 = Cnb[6]; Cnb32 = Cnb[7]; Cnb33 = Cnb[8];

  trC = Cnb11 + Cnb22 + Cnb33;

  P[0] = 1+trC;
  P[1] = 1+2*Cnb11-trC;
  P[2] = 1+2*Cnb22-trC;
  P[3] = 1+2*Cnb33-trC;

  //mP = max([P1 P2 P3 P4]);
  mP = P[0];
  sw = 0;
  for (size_t i = 1; i<4; i++){
    if (mP<P[i]){
      mP = P[i];
      sw = i;
    }
  }

  switch (sw){
    case 0:
      qnb[0] = sqrt(P[0])/2;
      qnb[1] = (Cnb32-Cnb23)/(4*qnb[0]);
      qnb[2] = (Cnb13-Cnb31)/(4*qnb[0]);
      qnb[3] = (Cnb21-Cnb12)/(4*qnb[0]);
    break;
    case 1:
      qnb[1] = sqrt(P[1])/2;
      qnb[2] = (Cnb21+Cnb12)/(4*qnb[1]);
      qnb[3] = (Cnb13+Cnb31)/(4*qnb[1]);
      qnb[0] = (Cnb32-Cnb23)/(4*qnb[1]);
    break;
    case 2:
      qnb[2] = sqrt(P[2])/2;
      qnb[3] = (Cnb32+Cnb23)/(4*qnb[2]);
      qnb[0] = (Cnb13-Cnb31)/(4*qnb[2]);
      qnb[1] = (Cnb21-Cnb12)/(4*qnb[2]);
    break;
    case 3:
      qnb[3] = sqrt(P[3])/2;
      qnb[0] = (Cnb21-Cnb12)/(4*qnb[3]);
      qnb[1] = (Cnb13+Cnb31)/(4*qnb[3]);
      qnb[2] = (Cnb32+Cnb23)/(4*qnb[3]);
    break;
    default:
    break;
  }

  iqn = 1 / matrix_modulus(qnb, 4);

   // normalize and negate if q0<0
   for (size_t i=0; i<=3; i++) {
     qnb[i] = qnb[i] * iqn;
     if (qnb[0] < 0)
       q[i] = -qnb[i];
     else{
       q[i] = qnb[i];
     }
   }
}




template <typename T>
static void __quat2dcm(T *Cnb, const T *q){
  T Cnb11, Cnb12, Cnb13;
  T Cnb21, Cnb22, Cnb23;
  T Cnb31, Cnb32, Cnb33;
  T Q1, Q2, Q3, Q4;
  T Q1_2, Q2_2, Q3_2, Q4_2;

  Q1 = q[0]; Q2 = q[1]; Q3 = q[2]; Q4 = q[3];
  Q1_2 = Q1*Q1;
  Q2_2 = Q2*Q2;
  Q3_2 = Q3*Q3;
  Q4_2 = Q4*Q4;
  Cnb11 = Q1_2+Q2_2-Q3_2-Q4_2;
  Cnb22 = Q1_2-Q2_2+Q3_2-Q4_2;
  Cnb33 = Q1_2-Q2_2-Q3_2+Q4_2;

  Cnb12 = 2*(Q2*Q3-Q1*Q4);
  Cnb13 = 2*(Q2*Q4+Q1*Q3);
  Cnb21 = 2*(Q2*Q3+Q1*Q4);
  Cnb23 = 2*(Q3*Q4-Q1*Q2);
  Cnb31 = 2*(Q2*Q4-Q1*Q3);
  Cnb32 = 2*(Q3*Q4+Q1*Q2);

  Cnb[0] = Cnb11; Cnb[1] = Cnb12; Cnb[2] = Cnb13;
  Cnb[3] = Cnb21; Cnb[4] = Cnb22; Cnb[5] = Cnb23;
  Cnb[6] = Cnb31; Cnb[7] = Cnb32; Cnb[8] = Cnb33;
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
void DCM2Euler(Vector3d<T> *eu, const  MatrixUnsafe<T> *Cnb){
  T *eu_int, *Cnb_int;
  eu_int = eu->getArray();
  Cnb_int = Cnb->getArray();
  __dcm2euler<T>(eu_int, Cnb_int);
}

template <typename T>
void DCM2Quat(Quaternion<T> *q, const  MatrixUnsafe<T> *Cnb){
  T *q_int, *Cnb_int;
  q_int = q->getArray();
  Cnb_int = Cnb->getArray();
  __dcm2quat<T>(q_int, Cnb_int);
}

template <typename T>
void Quat2DCM(MatrixUnsafe<T> *Cnb, const Quaternion<T> *q){
  T *q_int, *Cnb_int;
  q_int = q->getArray();
  Cnb_int = Cnb->getArray();
  __quat2dcm<T>(Cnb_int, q_int);
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

template <typename T>
bool isCorrect(const Quaternion<T> &q){
  if (
      isinf(q(0)) || isnan(q(0)) ||
      isinf(q(1)) || isnan(q(1)) ||
      isinf(q(2)) || isnan(q(2)) ||
      isinf(q(3)) || isnan(q(3))
     )
    return CH_FAILED;
  else
    return CH_SUCCESS;
}

} //namespace matrix

#endif /* QUATERNION_HPP_ */


