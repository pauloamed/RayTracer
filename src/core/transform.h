#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "bounds.h"

namespace rt3{

class Transform {
private:
  Matrix m, mInv;
public:

  Transform(const Matrix &m) : m(m), mInv(m.inverse()) { }

  Transform(const Matrix &m, const Matrix &mInv) 
    : m(m), mInv(mInv) {
  }

  Transform inverse(const Transform &t);

  Transform transpose(const Transform &t);

  bool operator==(const Transform &t) const;

  bool operator!=(const Transform &t) const;

  bool IsIdentity() const;

  const Matrix &GetMatrix() const;
  
  const Matrix &GetInverseMatrix() const;

  Point3f operator()(const Point3f &p) ;
  Vector3f operator()(const Vector3f &v) const;
  
  Ray operator()(const Ray &r) const;
  
  Bounds3f operator()(const Bounds3f &b) const;
  
  Transform operator*(const Transform &t2) const;

};


}

#endif