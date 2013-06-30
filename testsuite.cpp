#include <iostream>

#include "string.h"
#include "matrix.hpp"

/**
 * @brief   Fuzzy compare float values
 */
template <typename T>
bool fuzzy_compare(T reference, T val, T clearance){
  if((val > (reference + clearance)) || (val < reference - clearance))
    return false;
  else
    return true;
}

/**
 * @brief compare
 * @param src
 * @param ref
 * @return
 */
template <typename T>
bool compare(const Matrix<T> &src, const Matrix<T> &ref){
  if((src.getCol() != ref.getCol()) || (src.getRow() != ref.getRow()))
    return false;

  size_t len = ref.getCol() * ref.getRow();
  T *src_m = src.getArray();
  T *ref_m = ref.getArray();

  for (uint32_t i=0; i<len; i++){
    if (! fuzzy_compare(ref_m[i], src_m[i], 0.0001))
      return false;
  }
  return true;
}


/**
 * @brief gen_non_singular_array
 */
template <typename T>
static void nonsingular(T *a, size_t n){
  srand(time(NULL));
  T seed = rand() % 20 + 2;
  size_t i=0;

  for (i=0; i<(n*n); i++){
    a[i] = 1;
  }

  for (i=0; i<(n*n); i+=(n+1)){
    a[i] = seed++;
  }
}

/**
 *
 */
template <typename T>
static void identity(T *a, size_t n){
  size_t i=0;

  for (i=0; i<(n*n); i++){
    a[i] = 0;
  }

  for (i=0; i<(n*n); i+=(n+1)){
    a[i] = 1;
  }
}

/**
 * @brief identity_mul_test
 */
template <typename T>
static void identity_mul_test(Matrix<T> &A, Matrix<T> &I, Matrix<T> &Tmp){
  std::cout << "identity multiplication test size: " << A.getCol() << "\n";
  A.mul(I, Tmp);
  assert(compare(A, Tmp));

  I.mul(A, Tmp);
  assert(compare(A, Tmp));
}

template <typename T>
static void inv_test(Matrix<T> &A, Matrix<T> &B, Matrix<T> &I, Matrix<T> &Tmp){
  std::cout << "inversion test size: " << A.getCol() << "\n";

  Tmp = A;
  A.inverse();
  A.inverse();
  assert(compare(A, Tmp));

  A = Tmp;
  A.inverse();
  A.mul(Tmp, B);
  assert(compare(B, I));

  Tmp.mul(A, B);
  assert(compare(B, I));
}

/**
 * 
 */
static void base_test(void){
  //matrixDbgCheck((start > 0) && (start < finish), "");
  //int32_t result;
  uint32_t m = 1;
  size_t S = 0;

  double a[10000], b[10000], c[10000], i[10000];

  for (m=1; m<100; m++){

    S = sizeof(a[0]) * m*m;

    nonsingular(a, m);
    identity(i, m);

    memcpy(b, a, S);
    memcpy(c, a, S);

    Matrix<double> *A = new Matrix<double>(a, m, m, S);
    Matrix<double> *B = new Matrix<double>(b, m, m, S);
    Matrix<double> *TMP = new Matrix<double>(c, m, m, S);
    Matrix<double> *I = new Matrix<double>(i, m, m, S);

    identity_mul_test(*A, *I, *TMP);
    inv_test(*A, *B, *I, *TMP);

    delete B;
    delete A;
    delete TMP;
    delete I;
  }

  MatrixBuf<float, 3, 3> M;
  const float Mv[] = {1,2,3, 4,5,6, 7,8,9};
  M.init(Mv);

  MatrixBuf<float, 3, 3> N;
  const float Nv[] = {1,0,0, 0,1,0, 0,0,1};
  N.init(Nv);

  float mm[9];
  Matrix<float> R(mm, 3, 3, sizeof(mm));
  R = M + N;

  for (int i=0; i<9; i++){
    std::cout << mm[i];
  }

  R = ~M;
  std::cout << "\n";

  for (int i=0; i<9; i++){
    std::cout << mm[i];
  }

  /**/
  Vector<float, 3> V(Mv);
  R = N * V;
}

/**
 * @brief main
 * @return
 */
int main(void){
  base_test();
}
