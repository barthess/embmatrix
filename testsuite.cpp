#include <iostream>
#include <string>

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
 * 
 */
static void inversion_test(void){
  //matrixDbgCheck((start > 0) && (start < finish), "");

  double init1x1[1] = {2.0};
  double result1x1[1] = {0.5};

  Matrix<double> M(init1x1, 1, 1, sizeof(init1x1));
  MatrixBuf<double, 1, 1> tmp;

  M.inverse();
  std::cout << "inversion 1x1: ";
  if(fuzzy_compare(result1x1[0], M(0,0), 0.01))
    std::cout << "OK";
  else
    std::cout << "falied";
  std::cout << "\n";
}


int main(void){
  inversion_test();
}
