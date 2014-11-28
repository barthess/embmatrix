#ifndef MATRIX_OSAL_HPP_
#define MATRIX_OSAL_HPP_

void *matrix_malloc(size_t pool_index, size_t size);
void matrix_free(size_t pool_index, void *mem);
void matrixDbgCheck(bool a);
void matrixDbgPanic(const char *msg);
void matrixDbgPrint(const char *msg);

#endif /* MATRIX_OSAL_HPP_ */
