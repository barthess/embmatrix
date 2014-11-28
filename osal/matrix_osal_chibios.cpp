#include "hal.h"

#include "matrix_mempool.hpp"

void *matrix_malloc(size_t pool_index, size_t size) {
  void *ret;
  uint32_t start = chSysGetRealtimeCounterX();
  matrix_malloc_cnt++;

  osalDbgCheck(pool_index < MATRIX_MEMPOOL_LEN);
  osalDbgCheck(pool_array[pool_index].mp_object_size >= size);
  ret = chPoolAlloc(&pool_array[pool_index]);
  //matrixDbgCheck(NULL == ret);
  osalDbgCheck(NULL != ret);
  matrix_alloc_time += chSysGetRealtimeCounterX() - start;
  return ret;
}

void matrix_free(size_t pool_index, void *mem) {
  uint32_t start = chSysGetRealtimeCounterX();
  if (NULL != mem){
    matrix_free_cnt++;
    chPoolFree(&pool_array[pool_index], mem);
  }
  matrix_free_time += chSysGetRealtimeCounterX() - start;
}

void matrixDbgCheck(bool a){
  osalDbgCheck(a);
}

void matrixDbgPanic(const char *msg){
  osalSysHalt(msg);
}

void matrixDbgPrint(const char *msg){
  (void)msg;
}
