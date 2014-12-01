#ifndef EMBMATRIX_DISCOVERY_MATRIX_MEMPOOL_HPP_
#define EMBMATRIX_DISCOVERY_MATRIX_MEMPOOL_HPP_

#include "hal.h"

#define MATRIX_MEMPOOL_LEN          9
#define MATRIX_MEMPOOL_MIN_SIZE     (sizeof(float) * 4)

extern size_t matrix_malloc_cnt, matrix_free_cnt;
extern uint32_t matrix_alloc_time, matrix_free_time;
extern memory_pool_t pool_array[MATRIX_MEMPOOL_LEN];

void matrixMempoolStart(void);

#endif /* EMBMATRIX_DISCOVERY_MATRIX_MEMPOOL_HPP_ */
