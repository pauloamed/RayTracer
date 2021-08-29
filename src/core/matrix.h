#ifndef MATRIX_H
#define MATRIX_H

#include "math_base.h"

namespace rt3{

// template<typename T>
struct Matrix{
// private:
  vector<vector<real_type>> m;

// public:

  Matrix(const vector<vector<real_type>> &matrix):m(matrix){}

  Matrix transpose() const;
  Matrix inverse() const;

  bool isIdentity() const;

  friend bool operator==(const Matrix &a, const Matrix &b){
    return a.m == b.m;
  }

  };

}


#endif 