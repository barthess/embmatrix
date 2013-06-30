#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>

/**
 * @brief       Low level matrix class.
 * @note        Dangerouse. Use it if you want create interleaved matrixes.
 */
template<typename T>
class MatrixLL{
public:
  /**
   * @brief Constructor
   *
   * @param[in] m         pointer to a linear array of matrix data
   * @param[in] r         number of rows
   * @param[in] c         number of columns
   * @param[in] bufsize   size of data array in bytes. Need to check correctness
   *                      of sizes
   */
  MatrixLL(T *m, uint32_t r, uint32_t c, size_t bufsize){
    matrixDbgCheck(((0 != r) && (0 != c)), "Zero sizes forbidden");
    matrixDbgCheck(bufsize == (r * c * sizeof(T)), "Probable overflow");
    this->m = m;
    this->col = c;
    this->row = r;
  }

  /**
   * @brief Copy constructor. Forbidden.
   */
  MatrixLL(const MatrixLL &m);

  /**
   * Initialize matrix using vector of values starting from 0th element
   */
  void init(const T *v){
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++)
      this->m[i] = v[i];
  }

  /**
   * Initialize matrix using pattern
   */
  void init(const T pattern){
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++)
      this->m[i] = pattern;
  }

  /**
   * Return pointer to raw matrix data
   */
  T *getArray(void) const {
    return this->m;
  }

  /**
   * Return pointer to raw matrix data
   */
  uint32_t getCol(void) const {
    return this->col;
  }

  /**
   * Return pointer to raw matrix data
   */
  uint32_t getRow(void) const {
    return this->row;
  }

  /**
   * Modulus
   */
  T modulus(void) const {
    T R = 0;
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++)
      R += this->m[i] * this->m[i];
    return sqrt(R);
  }

  /**
   * normalize
   */
  void normalize(void){
    T R = this->modulus();
    matrixDbgCheck((R > 0), "divizion by zero");
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++)
      this->m[i] /= R;
  }

  /**
   * Invers matrix itself
   * The function returns 1 on success, 0 on failure.
   */
  int32_t inverse(void){
    matrixDbgCheck(this->col == this->row, "matrix must be square");
    return matrix_inverse(static_cast<int32_t>(this->col), this->m);
  }

  /**
   * Reshape matrix
   */
  void reshape(const uint32_t r, const uint32_t c){
    matrixDbgCheck((this->col * this->row) == (c * r), "resulted matrix size differ");
    this->col = c;
    this->row = r;
  }

  /**
   * Trnaspose matrix storing result in different matrix
   */
  void transpose(MatrixLL *result){
    matrixDbgCheck(result != this, "this functions can not work inplace");
    matrixDbgCheck((col == result->row) && (row == result->col),
        "matrix sizes incorrect");

    matrix_transpose(col, row, m, result->m);
  }

  /**
   * Trnaspose matrix
   */
  MatrixLL operator~ (void){
    T m[this->col * this->row];
    MatrixLL<T> result(m, this->col, this->row, sizeof(m));

    this->transpose(&result);
    return result;
  }

  /**
   * Transpose inplace
   * Matrix must be square or
   * one of dimensions must be equal to 1
   */
  void transpose(void){
    matrixDbgCheck((1 == col) || (1 == row) || (row == col),
        "matrix sizes unsupported");

    /* single dimmension vector simly reshaped */
    if((1 == col) || (1 == row)){
      uint32_t tmp = col;
      col = row;
      row = tmp;
    }

    /* square matrix can be transposed element by element */
    else{
      T tmp;
      uint32_t submatrix = 0;
      uint32_t i=0, k=0, p=0;
      for (submatrix=0; submatrix<(row - 1); submatrix++){
        for (i=submatrix+1; i<col; i++){
          k = i + submatrix*col;
          p = i*col + submatrix;
          tmp = m[k];
          m[k] = m[p];
          m[p] = tmp;
        }
      }
    }
  }

  /**
   *
   */
  void operator += (T v){
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++){
      m[i] += v;
    }
  }

  /**
   *
   */
  void operator += (const MatrixLL &M){
    matrixDbgCheck((M.col == col) && (M.row == row), "matrix sizes must be same");
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++){
      m[i] += M.m[i];
    }
  }

  /**
   *
   */
  MatrixLL operator + (const MatrixLL &M){
    T m[this->col * this->row];
    MatrixLL<T> result(m, this->col, this->row, sizeof(m));
    result = *this;
    result += M;
    return result;
  }

  /**
   *
   */
  void operator -= (T v){
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++){
      m[i] -= v;
    }
  }

  /**
   *
   */
  void operator -= (const MatrixLL &M){
    matrixDbgCheck((M.col == col) && (M.row == row), "matrix sizes must be same");
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++){
      m[i] -= M.m[i];
    }
  }

  /**
   *
   */
  MatrixLL operator - (const MatrixLL &M){
    T m[this->col * this->row];
    MatrixLL<T> result(m, this->col, this->row, sizeof(m));
    result = *this;
    result -= M;
    return result;
  }

  /**
   * Scale matrix elemets by constant
   */
  void operator *= (const T v){
    uint32_t len = col * row;
    for (uint32_t i=0; i<len; i++)
      this->m[i] *= v;
  }

  /**
   * @brief Subindex for Matrix elements assignation.
   * @param r
   * @param c
   * @return pointer to the element.
   */
  T& operator() (const uint32_t r, const uint32_t c){
    return this->m[calc_subindex(r,c)];
  }

  /**
   * @brief Subindex for Matrix element.
   * @param r
   * @param c
   * @return the element.
   */
  T operator() (uint32_t r, uint32_t c) const{
    return this->m[calc_subindex(r, c)];
  }

  /**
   * @brief   Multiply matrix themself by another matrix
   * @param[in] right     multiplier
   * @param[out] result   place result in it
   */
  void mul(const MatrixLL &right, MatrixLL &result){

    matrixDbgCheck(((this->col == right.row) &&
                (result.row == this->row) &&
                (result.col == right.col)), "sizes inconsistent");

    matrix_multiply(this->row, this->col, right.col,
                    this->m, right.m, result.m);
  }

  /**
   * @brief   Multiply operator
   * @param[in] right     multiplier
   */
  MatrixLL operator* (const MatrixLL &right){
    T m[this->row * right.col];
    MatrixLL<T> result(m, this->row, right.col, sizeof(m));

    this->mul(right, result);
    return result;
  }

  /**
   * @brief   Copy operator.
   * @details Check sizes and than copy data element by element
   */
  MatrixLL& operator= (const MatrixLL &src){
    if (this == &src)
      return *this;

    /* */
    matrixDbgCheck((src.col == this->col) && (src.row == this->row), "sizes mismatch");
    size_t len = this->col * this->row;
    for (uint32_t i=0; i<len; i++)
      this->m[i] = src.m[i];
    return *this;
  }

  /**
   * @brief   Compare operator. Compare sizes than compare data elements by elements.
   */
  bool operator== (const MatrixLL &src){
    if (this == &src)
      return true;

    if((src.col != this->col) || (src.row != this->row))
      return false;

    size_t len = this->col * this->row;
    for (uint32_t i=0; i<len; i++){
      if (this->m[i] != src.m[i])
        return false;
    }
    return true;
  }

  /**
   *
   */
  bool operator!= (const MatrixLL &src){
    return !(this == src);
  }

private:
  /**
   * @brief calculate subindex from row and col values
   */
  uint32_t calc_subindex(uint32_t r, uint32_t c) const {
    matrixDbgCheck((c < col) && (r < row), "overflow");
    return r*col + c;
  }

  uint32_t row;
  uint32_t col;
  T *m;
};


/**
 * Convenient class
 * representing matrix with automatically allocated static buffer
 */
template<typename T, int r, int c>
class MatrixBuf : public MatrixLL<T>{
protected:
  T m[r*c];

public:
  MatrixBuf(void) :
    MatrixLL<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = 0;
  }

  MatrixBuf(const T *initvector) :
    MatrixLL<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = initvector[i];
  }

  MatrixBuf(T pattern) :
    MatrixLL<T>(m, r, c, sizeof(m))
  {
    for (uint32_t i=0; i<(c*r); i++)
      m[i] = pattern;
  }

  /**
   * @brief   Copy operator.
   */
  MatrixBuf& operator= (const MatrixBuf &src){
    MatrixLL<T>::operator=(src);
    return *this;
  }
};


#endif /* MATRIX_H */
