#include "hal.h"

#include "matrix_osal.hpp"

extern size_t matrix_malloc_cnt, matrix_free_cnt;
extern uint32_t matrix_alloc_time, matrix_free_time;

static memory_pool_t pool_array[MATRIX_MEMPOOL_LEN];

void matrixMempoolStart(void){
  size_t pool_size = MATRIX_MEMPOOL_MIN_SIZE;

  for (size_t i=0; i<MATRIX_MEMPOOL_LEN; i++){
    chPoolObjectInit(&pool_array[i], pool_size, chCoreAllocI);
    pool_size *= 2;
  }
}

void *matrix_malloc(size_t pool_index, size_t size) {
  void *ret;
  uint32_t start = chSysGetRealtimeCounterX();
  matrix_malloc_cnt++;

  matrixDbgCheck(pool_index < MATRIX_MEMPOOL_LEN);
  matrixDbgCheck(pool_array[pool_index].mp_object_size >= size);
  ret = chPoolAlloc(&pool_array[pool_index]);
  //matrixDbgCheck(NULL == ret);
  matrixDbgCheck(NULL != ret);
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
