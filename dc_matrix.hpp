/*
 * direction cosine matrix
 */

#ifndef DC_MATRIX_HPP_
#define DC_MATRIX_HPP_

#include "matrix.hpp"

template <typename T>
class DCMatrix : public Matrix<T, 3, 3> {
public:
  DCMatrix(void) {
    this->m[0] = 1; this->m[1] = 0; this->m[2] = 0;
    this->m[3] = 0; this->m[4] = 1; this->m[5] = 0;
    this->m[6] = 0; this->m[7] = 0; this->m[8] = 1;
  }

  void Rx(void){;}

private:
  //const T init_vector[9] = {1,0,0, 0,1,0, 0,0,1};
};


#endif /* DC_MATRIX_HPP_ */
