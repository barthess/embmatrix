#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import math
import numpy as np

f = open("matrix_test.cpp", "w")

CurrentTest = 0
MAX_SIZE = 9
RAND_PASSES = 10

def array_to_cpp(nparray):
    st = ""
    for n in nparray:
        st += str(n)
        st += ","
    st = "{" + st[0:-1] + "}"
    return st

def gen_samples_mul(m, p, n, TA, TB):
    a = np.array(np.random.randint(-50, 50, m*p))
    b = np.array(np.random.randint(-50, 50, p*n))

    if not TA and not TB:
        c = np.dot(a.reshape(m,p), b.reshape(p,n))
    elif TA and not TB:
        c = np.dot(a.reshape(p,m).transpose(), b.reshape(p,n))
    elif not TA and TB:
        c = np.dot(a.reshape(m,p), b.reshape(n,p).transpose())
    else:
        c = np.dot(a.reshape(p,m).transpose(), b.reshape(n,p).transpose())
    c = c.reshape(m*n)

    return (a, b, c)

def write_function_mul(f, m, p, n, TA, TB):
    global CurrentTest
    typename = "int"

    print ("generate:", CurrentTest)
    (a,b,c) = gen_samples_mul(m, p, n, TA, TB)

    f.write("bool test" + str(CurrentTest) + "(void) {\n")
    f.write("  bool ret = false;\n")
    f.write("  %s a[] = %s;\n" % (typename, array_to_cpp(a)))
    f.write("  %s b[] = %s;\n" % (typename, array_to_cpp(b)))
    f.write("  %s c[] = %s;\n\n" % (typename, array_to_cpp(c)))
    if not TA and not TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, p))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, p, n))
        f.write("  Matrix<%s, %u, %u> C = A * B;\n\n" % (typename, m, n))
    elif TA and not TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, p, m))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, p, n))
        f.write("  Matrix<%s, %u, %u> C = ~A * B;\n\n" % (typename, m, n))
    elif not TA and TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, p))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, n, p))
        f.write("  Matrix<%s, %u, %u> C = A * ~B;\n\n" % (typename, m, n))
    else:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, p, m))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, n, p))
        f.write("  Matrix<%s, %u, %u> C = ~A * ~B;\n\n" % (typename, m, n))
    f.write("  ret = C == ref;\n\n")
    f.write("  if (!ret){\n")
    f.write("    matrix_fancy_print(A);\n")
    f.write("    matrix_fancy_print(B);\n")
    f.write("    matrix_fancy_print(C);\n  }\n")
    f.write("  return ret;\n")
    f.write("}\n\n")
    CurrentTest += 1





def gen_samples_add(m, n, sign, TA, TB):
    a = np.array(np.random.randint(-50, 50, m*n))
    b = np.array(np.random.randint(-50, 50, m*n))

    if sign == "-":
        if not TA and not TB:
            c = a.reshape(m,n) - b.reshape(m,n)
        elif TA and not TB:
            c = a.reshape(n,m).transpose() - b.reshape(m,n)
        elif not TA and TB:
            c = a.reshape(m,n) - b.reshape(n,m).transpose()
        else:
            c = a.reshape(n,m).transpose() - b.reshape(n,m).transpose()
    elif sign == "+":
        if not TA and not TB:
            c = a.reshape(m,n) + b.reshape(m,n)
        elif TA and not TB:
            c = a.reshape(n,m).transpose() + b.reshape(m,n)
        elif not TA and TB:
            c = a.reshape(m,n) + b.reshape(n,m).transpose()
        else:
            c = a.reshape(n,m).transpose() + b.reshape(n,m).transpose()
    else:
        raise ValueError("only '+' and '-' supported")

    c = c.reshape(m*n)
    return (a,b,c)


def write_function_add(f, m, n, sign, TA, TB):
    global CurrentTest
    typename = "int"

    print ("generate:", CurrentTest)
    (a,b,c) = gen_samples_add(m, n, sign, TA, TB)

    f.write("bool test" + str(CurrentTest) + "(void) {\n")
    f.write("  bool ret = false;\n")
    f.write("  %s a[] = %s;\n" % (typename, array_to_cpp(a)))
    f.write("  %s b[] = %s;\n" % (typename, array_to_cpp(b)))
    f.write("  %s c[] = %s;\n\n" % (typename, array_to_cpp(c)))
    if not TA and not TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> C = A %s B;\n\n" % (typename, m, n, sign))
    elif TA and not TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, n, m))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> C = ~A %s B;\n\n" % (typename, m, n, sign))
    elif not TA and TB:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, n, m))
        f.write("  Matrix<%s, %u, %u> C = A %s ~B;\n\n" % (typename, m, n, sign))
    else:
        f.write("  Matrix<%s, %u, %u> ref(c, sizeof(c));\n" % (typename, m, n))
        f.write("  Matrix<%s, %u, %u> A(a, sizeof(a));\n" % (typename, n, m))
        f.write("  Matrix<%s, %u, %u> B(b, sizeof(b));\n\n" % (typename, n, m))
        f.write("  Matrix<%s, %u, %u> C = ~A %s ~B;\n\n" % (typename, m, n, sign))
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

def gen_code_mul():
    for m in range(1, MAX_SIZE):
        for p in range(1, MAX_SIZE):
            for n in range(1, MAX_SIZE):
                write_function_mul(f, m, p, n, False, False)
                write_function_mul(f, m, p, n, True,  False)
                write_function_mul(f, m, p, n, False, True)
                write_function_mul(f, m, p, n, True,  True)

def gen_code_add():
    for m in range(1, MAX_SIZE):
        for n in range(1, MAX_SIZE):
            write_function_add(f, m, n, "-", False, False)
            write_function_add(f, m, n, "-", True,  False)
            write_function_add(f, m, n, "-", False, True)
            write_function_add(f, m, n, "-", True,  True)

            write_function_add(f, m, n, "+", False, False)
            write_function_add(f, m, n, "+", True,  False)
            write_function_add(f, m, n, "+", False, True)
            write_function_add(f, m, n, "+", True,  True)

for i in range(RAND_PASSES):
    # gen_code_mul()
    gen_code_add()


f.write("static const size_t total_test_cnt = " + str(CurrentTest) + ";\n")
f.write("static const mtf test_table[] = {\n")
i = 0
while i < CurrentTest:
    f.write("  test" + str(i) + ",\n")
    i += 1
f.write("};\n\n")

f.write("""

int main(){
    size_t speedtest = 1;
    while(speedtest--){
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
}
""")
