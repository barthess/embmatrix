#ifndef MATRIX_DBG_HPP_
#define MATRIX_DBG_HPP_

#if defined(CHIBI_OS)
  #define matrixDbgCheck(a,b) chDbgCheck(a,b)
  #include "misc_math.hpp"
#else
  #define matrixDbgCheck(a,b)
#endif

#endif /* MATRIX_DBG_HPP_ */
