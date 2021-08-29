#include "matrix.h"
#include <numeric>

namespace rt3{

  Matrix Matrix::transpose() const{
    vector<vector<real_type>> newM;
    for(int i = 0; i < m[0].size(); ++i){
      vector<real_type> row;
      for(int j = 0; j < m.size(); ++j){
        row.push_back(m[j][i]);
      }
      newM.push_back(row);
    }
    return Matrix(newM);
  }


  Matrix Matrix::inverse() const{

  }

  bool Matrix::isIdentity() const{
    assert(m.size() == m[0].size());
    int totActive = 0;
    for(auto &x : m){
      totActive += std::accumulate(x.begin(), x.end(), 0);
    }
    if(totActive != m.size()) return false;
    for(int i = 0; i < m.size(); ++i){
      if(m[i][i] != 1) return false;
    }
    return true;
  }

}