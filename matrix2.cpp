#include "matrix2.hpp"

float init1[4] = {1,0,0,1};


static void multiply(void){
  std::cout << "\n------- Multiply test --------\n";

  Matrix2<float, 3, 5> left(1);
  Matrix2<float, 5, 2> right(5);
  //Matrix2<float, 3, 2> result;

  //result = left * right;
  Matrix2<float, 3, 2>result = left * right;
  result.print();
}


static Matrix2<float, 3, 3> add(void){
  std::cout << "\n------- Addition test --------\n";

  Matrix2<float, 3, 3> left(1);
  Matrix2<float, 3, 3> right(5);
  Matrix2<float, 3, 3> right2(5);
  Matrix2<float, 3, 3> result;

  return (left + right + right2) * left;
  //Matrix2<float, 3, 3>result = left + right + right2;
  //result.print();
  //return result;
}


int main(void){
  Matrix2<float, 2, 2> m1(init1, sizeof(init1));
  Matrix2<float, 2, 2> m2(2);
  Matrix2<float, 2, 2> m3(1);
  Matrix2<float, 1, 2> m5(5);

  std::cout << "---------------\n";
  Matrix2<float, 2, 2> m4 = m2 + m1;
  (void)m4;
  //m5 = m3;
  //m5 = submatrix(m3);
  //m1.print();
  //m5.print();

  multiply();
  add();
}
