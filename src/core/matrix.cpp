#include "matrix.h"
#include <numeric>

namespace rt3{

  Matrix4x4 Matrix4x4::transpose() const{
    vector<vector<real_type>> newM;
    for(size_t i = 0; i < m[0].size(); ++i){
      vector<real_type> row;
      for(size_t j = 0; j < m.size(); ++j){
        row.push_back(m[j][i]);
      }
      newM.push_back(row);
    }
    return Matrix4x4(newM);
  }


  Matrix4x4 Matrix4x4::inverse() const{

  }

  bool Matrix4x4::isIdentity() const{
    assert(m.size() == m[0].size());
    int totActive = 0;
    for(auto &x : m) totActive += std::accumulate(x.begin(), x.end(), 0);
    if(totActive != m.size()) return false;
    for(size_t i = 0; i < m.size(); ++i){
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
    // (N x M) (O x P) = (N x P)
    int n = a.m.size();
    int m = a.m[0].size();
    int o = b.m.size();
    int p = b.m[0].size();
    assert(m == o);

    vector<vector<real_type>> newVals(n, vector<real_type>(p));
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < p; ++j){
        for(int k = 0; k < m; ++k){
          newVals[i][j] += a.m[i][k] * b.m[k][j];
        }
      }
    }

    return Matrix4x4(newVals);
  }

}