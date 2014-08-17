#include "hal.h"

#include "matrix_osal.hpp"
#include "matrix_mempool.hpp"

static memory_pool_t pool_array[MATRIX_MEMPOOL_LEN];


void matrixMempoolStart(void){
  size_t pool_size = MATRIX_MEMPOOL_MIN_SIZE;

  for (size_t i=0; i<MATRIX_MEMPOOL_LEN; i++){
    chPoolObjectInit(&pool_array[i], pool_size, chCoreAllocI);
    pool_size *= 2;
  }
}

void *matrix_malloc(size_t pool_index) {
  void *ret;
  //uint32_t start = chSysGetRealtimeCounterX();
  //matrix_malloc_cnt++;

  ret = chPoolAlloc(&pool_array[pool_index]);
  matrixDbgCheck(NULL != ret);
  //matrix_alloc_time += chSysGetRealtimeCounterX() - start;
  return ret;
}

void matrix_free(void *mem, size_t pool_index) {
  //uint32_t start = chSysGetRealtimeCounterX();
  if (NULL != mem){
    //matrix_free_cnt++;
    chPoolFree(&pool_array[pool_index], mem);
  }
  //matrix_free_time += chSysGetRealtimeCounterX() - start;
}
