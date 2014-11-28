#include <iostream>
#include <cstdlib>
#include <assert.h>

void matrixDbgPrint(const char *msg) {
  std::cout << msg;
}

void matrixDbgPanic(const char *msg) {
  matrixDbgPrint(msg);
  throw 0;
  exit(1);
}

void matrixDbgCheck(bool c) {
  if (!c){
    matrixDbgPanic("Matrix error!");
  }
}

void *matrix_malloc(size_t pool_index, size_t size) {
  (void)pool_index;

  return malloc(size);
}

static inline void matrix_free(size_t pool_index, void *mem) {
  (void)pool_index;

  free(mem);
}

