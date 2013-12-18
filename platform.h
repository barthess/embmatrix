#ifndef MATRIX_DBG_HPP_
#define MATRIX_DBG_HPP_

#if defined(_CHIBIOS_RT_)
  #define matrixDbgCheck(a,b) chDbgCheck(a,b)
  #include "misc_math.hpp"
  #define matrixDbgPrint(msg) {;}
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

#endif /* MATRIX_DBG_HPP_ */
