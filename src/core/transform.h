#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "bounds.h"

namespace rt3{

class Transform {
private:
  Matrix m, mInv;
public:

  Transform():m(Matrix::getIdentity(4)), mInv(m){}

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

  Point3f apply(const Point3f &p) const;
  Vector3f apply(const Vector3f &v) const;
  Ray apply(const Ray &r) const;
  Bounds3f apply(const Bounds3f &b) const;  
  
  Transform update(const Transform &t2) const;

  static Transform getTranslationMatrix(const Vector3f &v);
  static Transform getRotationMatrix(const Point3f &v);
  static Transform getScalingMatrix(const Point3f &v);
  static Transform getIdentityMatrix();
};


const shared_ptr<Transform> IDENTITY = make_shared<Transform>(Transform::getIdentityMatrix());


}

#endif