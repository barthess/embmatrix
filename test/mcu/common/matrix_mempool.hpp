#ifndef MATRIX_MEMPOOL_HPP_
#define MATRIX_MEMPOOL_HPP_

#define MATRIX_MEMPOOL_LEN          10
#define MATRIX_MEMPOOL_MIN_SIZE     (sizeof(float) * 4)

void matrixMempoolStart(void);
void *matrix_malloc( size_t pool_index);
void matrix_free(void *mem, size_t pool_index);

#endif /* MATRIX_MEMPOOL_HPP_ */
