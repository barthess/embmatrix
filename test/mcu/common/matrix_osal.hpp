#ifndef MATRIX_OSAL_HPP_
#define MATRIX_OSAL_HPP_

#if defined(_CHIBIOS_RT_)
#include "gcc_stubs.hpp"
#include "misc_math.hpp"

#define matrixDbgCheck(a)           osalDbgCheck(a)
#define matrixDbgPanic(msg)         osalSysHalt(msg)
#define matrixDbgPrint(msg)         {;}

#define MATRIX_MEMPOOL_LEN          9
#define MATRIX_MEMPOOL_MIN_SIZE     (sizeof(float) * 4)

void matrixMempoolStart(void);
void *matrix_malloc( size_t pool_index, size_t size);
void matrix_free(size_t pool_index, void *mem);

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

#endif /* MATRIX_OSAL_HPP_ */
