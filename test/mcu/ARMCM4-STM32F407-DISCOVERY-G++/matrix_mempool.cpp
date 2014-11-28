#include "hal.h"

#include "matrix_mempool.hpp"

size_t matrix_malloc_cnt, matrix_free_cnt;
uint32_t matrix_alloc_time, matrix_free_time;
memory_pool_t pool_array[MATRIX_MEMPOOL_LEN];

void matrixMempoolStart(void){
  size_t pool_size = MATRIX_MEMPOOL_MIN_SIZE;

  for (size_t i=0; i<MATRIX_MEMPOOL_LEN; i++){
    chPoolObjectInit(&pool_array[i], pool_size, chCoreAllocI);
    pool_size *= 2;
  }
}
