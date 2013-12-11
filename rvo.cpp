#include "matrix.hpp"
#include <iostream>

//using namespace matrix;

//static Matrix<double, 1, 1> v1(1);
//static Matrix<double, 1, 1> v2(10);
//static Matrix<double, 1, 1> v3;

//int main(void){
//  v3 = v1 + v2;
//  std::cout << v3(0,0);
//  return v3(0,0);
//}


template<typename T>
class Base {
public:
  Base(void){
    std::cout << "base constructor default\n";
  }

  Base(T arg){
    std::cout << "base constructor valued\n";
    this->m = arg;
  }

  Base(const Base &b){
    std::cout << "base copy constructor\n";
    this->m = b.m;
  }

  Base<T> operator + (const Base &b){
    Base<T> ret;
    std::cout << "base operator +\n";
    ret.m = this->m + b.m;
    return ret;
  }

  Base& operator = (const Base &src){
    std::cout << "base operator =\n";
    if (this == &src)
      return *this;

    /* */
    this->m = src.m;
    return *this;
  }

  Base(Base &&that){
    std::cout << "base move constructor\n";
    m = that.m;
    that.m = 0;
  }

public:
  T m;
};

template <typename T>
Base<T> operator + (const Base<T> &b, const Base<T> &a){
  Base<T> ret;
  std::cout << "function +\n";
  ret.m = a.m + b.m;
  return ret;
}

template<typename T>
class Child : public Base<T> {
public:
  Child(void) : Base<T>(){
    std::cout << "child constructor default\n";
  }

  Child(T arg) : Base<T>(arg){
    std::cout << "child constructor valued\n";
    n = arg;
  }

public:
  T n;
};

int main(void) {
  std::cout << "Hello World!\n";
  Base<double> obj1(2);
  Base<double> obj2 = obj1;
  Base<double> obj3(3);
  std::cout << "----------------------\n";
  Child<double> obj4(8);
  Child<double> obj5(7);

  std::cout << "----------------------\n";
  obj3 = obj1 + obj2 + obj4;
  //obj3 = obj2;
  //obj4 = obj1;
  obj4 = obj4;// + obj5;
  std::cout << obj3.m << "\n";
}




