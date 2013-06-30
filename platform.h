#ifndef MATRIX_DBG_HPP_
#define MATRIX_DBG_HPP_

#if defined(_CHIBIOS_RT_)
  #define matrixDbgCheck(a,b) chDbgCheck(a,b)
  #include "misc_math.hpp"
#else
  #include <iostream>
  #include <cstdlib>
  #include <assert.h>
  #include <math.h>
  #define matrixDbgCheck(c, msg) {              \
    if (!(c)){                                  \
      std::cout << msg;                         \
      exit(1);                                  \
    }                                           \
  }
#endif

#endif /* MATRIX_DBG_HPP_ */
