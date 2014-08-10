#ifndef MISC_MATH_H_
#define MISC_MATH_H_

#include "math.h"

/**
 * Fast hardware square root
 */
__attribute__((always_inline)) __STATIC_INLINE float __VSQRT(float value)
{
  float result;
  __ASM volatile ("vsqrt.f32 %0, %1" : "=w"(result) : "w"(value));
  return(result);
}

/**
 * Overload function
 */
inline float sqrt(float v){
#if CORTEX_USE_FPU
  return __VSQRT(v);
#else
  return sqrtf(v);
#endif /* CORTEX_USE_FPU */
}

inline float cos(float v){
  return cosf(v);
}

inline float acos(float v){
  return acosf(v);
}

inline float sin(float v){
  return sinf(v);
}

inline float asin(float v){
  return asinf(v);
}

inline float tan(float v){
  return tanf(v);
}

inline float atan(float v){
  return atanf(v);
}

inline float atan2(float x, float y){
  return atan2f(x, y);
}

inline float fabs(float x){
  return fabsf(x);
}

inline float pow(float x, float y){
  return powf(x, y);
}

/**
 * Clamper function implementation
 */
template<typename T>
T __putinrange_impl(T v, T vmin, T vmax){

  T tmp;

  /* protection from stupidity */
  if(vmin >= vmax){
    tmp  = vmax;
    vmax = vmin;
    vmin = tmp;
  }

  /* brute comparison */
  if (v <= vmin)
    return vmin;
  else if (v >= vmax)
    return vmax;
  else
    return v;
}

/**
 * Clamper function wrapper
 */
template<typename T, typename T2, typename T3>
T putinrange(T v, T2 vmin, T3 vmax){
  return __putinrange_impl(v,
                           static_cast<typeof(v)>(vmin),
                           static_cast<typeof(v)>(vmax));
}

/**
 *
 */
template<typename Type>
Type min(Type v1, Type v2){

  if (v1 < v2)
    return v1;
  else
    return v2;
}

/**
 *
 */
template<typename Type>
Type max(Type v1, Type v2){

  if (v1 > v2)
    return v1;
  else
    return v2;
}

/**
 *
 */
template <typename T>
inline void enu2nue(T *v){
  T tmp;
  tmp  = v[0];
  v[0] = v[1];
  v[1] = v[2];
  v[2] = tmp;
}

/**
 *
 */
template <typename T>
inline void nwu2nue(T *v){
  T tmp;
  tmp  = v[2];
  v[2] = -v[1];
  v[1] = tmp;
}

/**
 *
 */
template <typename T>
inline void nue2nwu(T *v){
  T tmp;
  tmp  = v[1];
  v[1] = -v[2];
  v[2] = tmp;
}

/**
 *
 */
template <typename T>
inline void ned2nue(T *v){
  /* it is equivalent to nwu2nwu */
  nue2nwu(v);
}

/**
 *
 */
template <typename T>
inline void nue2ned(T *v){
  /* it is equivalent to nwu2nue */
  nwu2nue(v);
}

/**
 * Integer power function.
 */
template <typename T>
T powi(T base, uint32_t exp){
  T result = 1;
  while (exp > 0){
    result *= base;
    exp--;
  }
  return result;
}

/**
 * @brief       Multiplying value by polynomial.
 *
 * @param[in] poly    Array of polynomial coefficients (MSB first)
 * @param[in] len     Length of polynomial array
 * @param[in] val     This value will be multiplied by polynomial
 *
 * @return            Result of multiplication.
 */
template <typename T>
T PolyMul(const T *poly, size_t len, T val){
  T result;

  len--;
  result = poly[len];

  if (0 == len)
    return result;

  while (len--){
    result += poly[len] * val;
    val *= val;
  }

  return result;
}

/* mod(a, 2*pi) is the remainder you get when you divide a by 2*pi;
that is, subtract the largest multiple of 2*pi less than a from a,
and that's the answer. */
static inline float fmodulo(float x, float y){
  return x - y * floorf(x/y);
}
static inline double fmodulo(double x, double y){
  return x - y * floor(x/y);
}

uint32_t sqrti(uint32_t x);

#endif /* MISC_MATH_H_ */