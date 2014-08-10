#ifndef MATRIX_PRIMITIVES_H
#define MATRIX_PRIMITIVES_H

#include <stdint.h>
#include <string.h> // for memcpy()

namespace matrix {

/*
 ******************************************************************************
 * LOW LEVEL
 * Hi speed functions.
 * WARNING!!! no size checks
 ******************************************************************************
 */

/**
 * @brief   Matrix modulus.
 */
template <typename T>
T matrix_modulus(const T *A, size_t len){
  T R = 0;
  for (size_t i=0; i<len; i++)
    R += A[i] * A[i];
  return sqrt(R);
}

/**
 * @brief   Normalize matrix inplace.
 */
template <typename T>
void matrix_normalize(T *A, size_t len){
  T R = matrix_modulus(A, len);
  for (size_t i=0; i<len; i++)
    A[i] /= R;
}

/**
 * @brief     transpose matrix A(m x n) to B(n x m)
 */
template <typename T>
void matrix_transpose(size_t m, size_t n, const T *A, T *B){
  size_t i, j;
  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      B[j*m + i] = A[i*n + j];
}

/**
 * @brief   multiply matrix A(m x p) by  B(p x n), put result in C(m x n)
 */
template <typename T>
void matrix_multiply(size_t m, size_t p, size_t n,
                     const T *A, const T *B, T *C){
  size_t i, j, k;
  T tmp;

  for(i=0; i<m; i++){     //each row in A
    for(j=0; j<n; j++){   //each column in B
      tmp = 0;
      for(k=0; k<p; k++){ //each element in row A & column B
        tmp += A[i*p + k] * B[k*n + j];
      }
      C[i*n + j] = tmp;
    }
  }
}

/**
 * @brief   Copy function
 */
//template <typename T>
//void matrix_copy(size_t m, size_t n, const T *A, T *B){
//  memcpy(B, A, (m * n * sizeof(A[0])));
//}

// Matrix Inversion Routine from http://www.arduino.cc/playground/Code/MatrixMath
// * This function inverts a matrix based on the Gauss Jordan method.
// * Specifically, it uses partial pivoting to improve numeric stability.
// * The algorithm is drawn from those presented in
//   NUMERICAL RECIPES: The Art of Scientific Computing.
// * The function returns 1 on success, 0 on failure.
// * NOTE: The argument is ALSO the result matrix, meaning the input matrix is REPLACED

// A = input matrix AND result matrix
// n = number of rows = number of columns in A (n x n)
template <typename T>
int matrix_inverse(int n, T *A){

  int pivrow = 0; // keeps track of current pivot row
  int k,i,j;      // k: overall index along diagonal; i: row index; j: col index
  int pivrows[n]; // keeps track of rows swaps to undo at end
  T tmp;    // used for finding max value and making column swaps

  for (k=0; k<n; k++){
    // find pivot row, the row with biggest entry in current column
    tmp = 0;
    for (i=k; i<n; i++){
      if (fabs(A[i*n + k]) >= tmp){ // 'Avoid using other functions inside abs()?'
        tmp = fabs(A[i*n + k]);
        pivrow = i;
      }
    }

    // check for singular matrix
    if (A[pivrow*n + k] == 0)
      return 0; //Inversion failed due to singular matrix

    // Execute pivot (row swap) if needed
    if (pivrow != k) {
      for (j=0; j<n; j++) {// swap row k with pivrow
        tmp = A[k*n + j];
        A[k*n + j] = A[pivrow*n + j];
        A[pivrow*n + j] = tmp;
      }
    }
    pivrows[k] = pivrow;  // record row swap (even if no swap happened)

    tmp = 1 / A[k*n + k];  // invert pivot element
    A[k*n + k] = 1;    // This element of input matrix becomes result matrix

    // Perform row reduction (divide every element by pivot)
    for (j=0; j<n; j++)
      A[k*n + j] = A[k*n + j] * tmp;

    // Now eliminate all other entries in this column
    for (i=0; i<n; i++){
      if (i != k){
        tmp = A[i*n + k];
        A[i*n + k] = 0;  // The other place where in matrix becomes result mat
        for (j=0; j<n; j++)
          A[i*n + j] = A[i*n + j] - A[k*n + j] * tmp;
      }
    }
  }

  // Done, now need to undo pivot row swaps by doing column swaps in reverse order
  for (k=n-1; k >= 0; k--){
    if (pivrows[k] != k){
      for (i = 0; i < n; i++){
        tmp = A[i*n + k];
        A[i*n + k] = A[i*n + pivrows[k]];
        A[i*n + pivrows[k]] = tmp;
      }
    }
  }
  return 1;
}


/////////////////////////////////////////////////////////////////////////
// Old vector3d code.
// Need to replace it by new matrix code
/////////////////////////////////////////////////////////////////////////

/**
 * @brief   calcuate vector dot-product  c = a . b
 */
template <typename T>
T vector3d_dot(const T *a, T *b) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/**
 * @brief   calcuate vector cross-product  c = a x b
 */
template <typename T>
void vector3d_cross(const T *a, const T *b, T *c){
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
}

/**
 * @brief   convert vector to a vector with same direction and modulus 1
 */
template <typename T>
void vector3d_normalize(T* v){
  T R = matrix_modulus(v, 3);
  v[0] /= R;
  v[1] /= R;
  v[2] /= R;
}

/**
 * @brief   calcuate vector scalar-product  b = s x a
 */
template <typename T>
void vector3d_scale(const T s, const T *a, T *b){
  b[0] = s*a[0];
  b[1] = s*a[1];
  b[2] = s*a[2];
}

/**
 * @brief   calcuate vector sum   c = a + b
 */
template <typename T>
void vector3d_add(const T *a, const T *b, T *c){
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
}

/**
 * @brief   calcuate vector substraction c = a - b
 */
template <typename T>
void vector3d_sub(const T *a, const T *b, T *c){
  c[0] = a[0] - b[0];
  c[1] = a[1] - b[1];
  c[2] = a[2] - b[2];
}

} //namespace matrix

#endif /* MATRIX_PRIMITIVES_H */
