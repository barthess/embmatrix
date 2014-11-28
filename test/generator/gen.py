#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import math
import numpy as np

f = open("matrix_test.cpp", "w")


CurrentTest = 0
MAX_SIZE = 5


def array_to_cpp(nparray):
    st = ""
    for n in nparray:
        st += str(n)
        st += ","
    st = "{" + st[0:-1] + "}"
    return st

def write_function(f, m, p, n, typename):
    global CurrentTest

    f.write("bool test" + str(CurrentTest) + "(void) {\n")
    a = np.array(np.arange(m*p))
    b = np.array(np.arange(p*n))
    c = np.dot(a.reshape(m,p), b.reshape(p,n))
    # print (a.reshape(i,j))
    # print (b.reshape(i,j))
    # print (c)
    c = c.reshape(m*n)

    f.write("  bool ret = false;\n")
    f.write("  %s a[] = %s;\n" % (typename, array_to_cpp(a)))
    f.write("  %s b[] = %s;\n" % (typename, array_to_cpp(b)))
    f.write("  %s c[] = %s;\n\n" % (typename, array_to_cpp(c)))
    f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
    f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, p))
    f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n" % (typename, p, n))
    f.write("  Matrix<%s, %u, %u> C;\n\n" % (typename, m, n))
    f.write("  C = A * B;\n")
    f.write("  ret = C == ref;\n\n")
    f.write("  if (!ret){\n")
    f.write("    matrix_dbg_print(A);\n")
    f.write("    matrix_dbg_print(B);\n")
    f.write("    matrix_dbg_print(C);\n  }\n")
    f.write("  return ret;\n")
    f.write("}\n\n")
    CurrentTest += 1


f.write("""#include "matrix.hpp"
#include "matrix_dbg.hpp"
#include "matrix_osal_pc.cpp"
using namespace matrix;

typedef bool (*mtf)(void);

""")

for m in range(1, MAX_SIZE):
    for n in range(1, MAX_SIZE):
        for p in range(1, MAX_SIZE):
            write_function(f, m, n, p, "unsigned int")

f.write("static const size_t total_test_cnt = " + str(CurrentTest) + ";\n")
f.write("static const mtf test_table[] = {\n")
i = 0
while i < CurrentTest:
    f.write("  test" + str(i) + ",\n")
    i += 1
f.write("};\n\n")

f.write("""

int main(){
    bool status = false;
    for (size_t i=0; i<total_test_cnt; i++) {
        std::cout << "test: " << i << std::endl;
        status = test_table[i]();
        if (false == status){
            printf("failed\\n");
            exit(1);
        }
    }
    printf("success\\n");
}
""")
