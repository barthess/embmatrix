#ifndef MATRIX_PLATFORM_HPP_
#define MATRIX_PLATFORM_HPP_

#if defined(_CHIBIOS_RT_)
#include "gcc_stubs.hpp"
#include "misc_math.hpp"

extern MemoryHeap MatrixHeap;
extern int matrix_alloc_cnt;

#define matrixDbgCheck(a,b) chDbgCheck(a,b)
#define matrixDbgPrint(msg) {;}

static inline void *matrix_malloc(size_t len) {
  matrix_alloc_cnt++;
  return chHeapAlloc(&MatrixHeap, len);
}
static inline void matrix_free(void *p) {
  matrix_alloc_cnt--;
  chHeapFree(p);
}

#else
  #include <iostream>
  #include <cstdlib>
  #include <assert.h>
  #include <math.h>
  #define matrixDbgCheck(c, msg) {              \
    if (!(c)){                                  \
      std::cout << msg;                         \
      throw 0;                                  \
      exit(1);                                  \
    }                                           \
  }
  #define matrixDbgPrint(msg) { std::cout << msg; }
#endif

#endif /* MATRIX_PLATFORM_HPP_ */
