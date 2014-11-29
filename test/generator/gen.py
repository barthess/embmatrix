#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import math
import numpy as np

f = open("matrix_test.cpp", "w")

CurrentTest = 0
MAX_SIZE = 5
RAND_PASSES = 3

def array_to_cpp(nparray):
    st = ""
    for n in nparray:
        st += str(n)
        st += ","
    st = "{" + st[0:-1] + "}"
    return st

def gen_samples_uint_arange(m, p, n):
    a = np.array(np.arange(m*p))
    b = np.array(np.arange(p*n))
    c = np.dot(a.reshape(m,p), b.reshape(p,n))
    c = c.reshape(m*n)
    return (a, b, c)

def gen_samples_int_rand(m, p, n):
    a = np.array(np.random.randint(-50, 50, m*p))
    b = np.array(np.random.randint(-50, 50, p*n))
    c = np.dot(a.reshape(m,p), b.reshape(p,n))
    c = c.reshape(m*n)
    return (a, b, c)

def write_function(f, m, p, n, typename, gen_samples_func):
    global CurrentTest

    print ("generate:", CurrentTest)
    (a,b,c) = gen_samples_func(m, p, n)

    f.write("bool test" + str(CurrentTest) + "(void) {\n")
    f.write("  bool ret = false;\n")
    f.write("  %s a[] = %s;\n" % (typename, array_to_cpp(a)))
    f.write("  %s b[] = %s;\n" % (typename, array_to_cpp(b)))
    f.write("  %s c[] = %s;\n\n" % (typename, array_to_cpp(c)))
    f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
    f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, p))
    f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, p, n))
    f.write("  Matrix<%s, %u, %u> C = A * B;\n\n" % (typename, m, n))
    f.write("  ret = C == ref;\n\n")
    f.write("  if (!ret){\n")
    f.write("    matrix_fancy_print(A);\n")
    f.write("    matrix_fancy_print(B);\n")
    f.write("    matrix_fancy_print(C);\n  }\n")
    f.write("  return ret;\n")
    f.write("}\n\n")
    CurrentTest += 1


f.write("""#include "matrix.hpp"
#include "matrix_dbg.hpp"
#include "matrix_osal_pc.cpp"
using namespace matrix;

typedef bool (*mtf)(void);

""")

def gen_code(typename, func):
    for m in range(1, MAX_SIZE):
        for n in range(1, MAX_SIZE):
            for p in range(1, MAX_SIZE):
                write_function(f, m, n, p, typename, func)

gen_code("unsigned int", gen_samples_uint_arange)
for i in range(RAND_PASSES):
    gen_code("int", gen_samples_int_rand)


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
            printf("-----------------------------------\\n");
            printf("failed\\n");
            exit(1);
        }
    }
    printf("-----------------------------------\\n");
    printf("success\\n");
}
""")
