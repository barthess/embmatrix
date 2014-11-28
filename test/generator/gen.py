#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import math
import numpy as np

f = open("embmatrix_test.cpp", "w")

f.write("""#include "matrix.hpp"
#include "matrix_compare.hpp"
using namespace matrix

#define MATRIX_SUCCESS      false
#define MATRIX_FAILED       true

""")

CurrentTest = 0
COL_MAX = 5
ROW_MAX = 5

i = 5
j = 5

def array_to_cpp(nparray):
    st = ""
    for n in nparray:
        st += str(n)
        st += ","
    st = "{" + st[0:-1] + "}"
    return st

f.write("void test" + str(CurrentTest) + "(void) {\n")
a = np.array(np.arange(5*5))
b = np.array(np.arange(5*5))
c = np.dot(a.reshape(5,5), b.reshape(5,5))
c = c.reshape(5*5)

f.write("unsigned int a[] = " + array_to_cpp(a) + ";\n")
f.write("unsigned int b[] = " + array_to_cpp(b) + ";\n")
f.write("unsigned int c[] = " + array_to_cpp(c) + ";\n")
f.write("Matrix<unsigned int, 5, 5> ref(c);\n")
f.write("Matrix<unsigned int, 5, 5> A(a);\n")
f.write("Matrix<unsigned int, 5, 5> B(b);\n")
f.write("Matrix<unsigned int, 5, 5> C;\n")
f.write("C = A * B;\n")
f.write("return C == ref;\n")
f.write("}\n")

f.write("""

int main(){
    if (MATRIX_FAILED == test0())
        printf("failed");
    else
        printf("sucess");
}
""")
