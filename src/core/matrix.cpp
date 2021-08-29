#include "matrix.h"
#include <numeric>

namespace rt3{

  Matrix4x4 Matrix4x4::transpose() const{
    vector<vector<real_type>> newM;
    for(size_t i = 0; i < 4; ++i){
      vector<real_type> row;
      for(size_t j = 0; j < 4; ++j){
        row.push_back(m[j][i]);
      }
      newM.push_back(row);
    }
    return Matrix4x4(newM);
  }


  Matrix4x4 Matrix4x4::inverse() const{

  }

  bool Matrix4x4::isIdentity() const{
    int totActive = 0;
    for(auto &x : m) for(int i = 0; i < 4; ++i) totActive += x[i];
    if(totActive != 4) return false;
    for(size_t i = 0; i < 4; ++i){
      if(m[i][i] != 1) return false;
    }
    return true;
  }

  Matrix4x4 Matrix4x4::getIdentity(){
    int n = 4;
    vector<vector<real_type>> m(n, vector<real_type>(n));
    for(int i = 0; i < n; ++i) m[i][i] = 1;
    return Matrix4x4(m);
  }

  Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b){
    vector<vector<real_type>> newVals(4, vector<real_type>(4));
    for(int i = 0; i < 4; ++i){
      for(int j = 0; j < 4; ++j){
        for(int k = 0; k < 4; ++k){
          newVals[i][j] += a.m[i][k] * b.m[k][j];
        }
      }
    }

    return Matrix4x4(newVals);
  }

}