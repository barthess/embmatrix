#include "matrix2.hpp"

float init1[4] = {1,0,0,1};

int main(void){
  Matrix2<float, 2, 2> m1(init1, sizeof(init1));
  Matrix2<float, 2, 2> m2(2);
  Matrix2<float, 2, 2> m3(1);
  Matrix2<float, 1, 2> m5(5);

  std::cout << "---------------\n";
  Matrix2<float, 2, 2> m4 = m2 + m1;
  (void)m4;
  m5 = submatrix(m3);
  //m1.print();
  m5.print();
}
