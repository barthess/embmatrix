#ifndef MATRIX_OSAL_HPP_
#define MATRIX_OSAL_HPP_

#if defined(_CHIBIOS_RT_)
#include "gcc_stubs.hpp"
#include "misc_math.hpp"

extern memory_heap_t MatrixHeap;
extern size_t matrix_malloc_cnt, matrix_free_cnt;
extern uint32_t matrix_alloc_time, matrix_free_time;
extern memory_pool_t matrix_pool;

#define matrixDbgCheck(a) osalDbgCheck(a)
#define matrixDbgPrint(msg) {;}

static inline void *matrix_malloc(size_t len) {
  void *ret;
  uint32_t start = chSysGetRealtimeCounterX();
  matrix_malloc_cnt++;
  //ret = chHeapAlloc(&MatrixHeap, len);
  ret = chPoolAlloc(&matrix_pool);  (void)len;
  osalDbgCheck(NULL != ret);
  matrix_alloc_time += chSysGetRealtimeCounterX() - start;
  return ret;
}

static inline void matrix_free(void *p) {
  uint32_t start = chSysGetRealtimeCounterX();
  if (NULL != p){
    matrix_free_cnt++;
    //chHeapFree(p);
    chPoolFree(&matrix_pool, p);
  }
  matrix_free_time += chSysGetRealtimeCounterX() - start;
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

#endif /* MATRIX_OSAL_HPP_ */
