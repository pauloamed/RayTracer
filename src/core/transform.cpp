#include "transform.h"

namespace rt3{

Transform Transform::inverse() {
    return Transform(mInv, m);
}

Transform Transform::transpose() {
    return Transform(m.transpose(), mInv.transpose());
}

bool Transform::operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
}

bool Transform::operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
}

bool Transform::IsIdentity() const { return m.isIdentity(); }

const Matrix4x4 &Transform::GetMatrix() const { return m; }

const Matrix4x4 &Transform::GetInverseMatrix() const { return mInv; }

Point3f Transform::apply(const Point3f &p) const{
  return customMult(m, p);
}

Vector3f Transform::apply(const Vector3f &v) const{
  return customMult(m, v);
}

Ray Transform::apply(const Ray &r) const{
  auto newD = customMult(m, r.d);
  auto newO = customMult(m, r.o);
  return Ray(newD, newO);
}

Bounds3f Transform::apply(const Bounds3f &b) const{
  auto newMinP = customMult(m, b.minPoint);
  auto newMaxP = customMult(m, b.maxPoint);
  return Bounds3f(newMinP, newMaxP);
}

Transform Transform::update(const Transform &t2) const{
  return Transform(m * t2.m);
}


Transform Transform::getTranslationMatrix(const Vector3f &v){
  vector<vector<real_type>> m = {
    {1, 0, 0, v.at(0)},
    {0, 1, 0, v.at(1)},
    {0, 0, 1, v.at(2)},
    {0, 0, 0, 1}
  };
  return Transform(Matrix4x4(m));
}

Transform Transform::getRotationMatrix(const Point3f &v){
  return Transform();
}

Transform Transform::getScalingMatrix(const Point3f &p){
  vector<vector<real_type>> m = {
    {p.at(0), 0, 0, 0},
    {0, p.at(1), 0, 0},
    {0, 0, p.at(2), 0},
    {0, 0, 0, 1}
  };
  return Transform(Matrix4x4(m));
}


Transform Transform::getIdentityMatrix(){
  return Transform();
}



}