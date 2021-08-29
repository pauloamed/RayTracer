#include "transform.h"

namespace rt3{

Transform Transform::inverse(const Transform &t) {
    return Transform(t.mInv, t.m);
}

Transform Transform::transpose(const Transform &t) {
    return Transform(t.m.transpose(), t.mInv.transpose());
}

bool Transform::operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
}

bool Transform::operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
}

bool Transform::IsIdentity() const { return m.isIdentity(); }

const Matrix &Transform::GetMatrix() const { return m; }

const Matrix &Transform::GetInverseMatrix() const { return mInv; }

Point3f Transform::operator()(const Point3f &p){
  return p * m;
}

Vector3f Transform::operator()(const Vector3f &v) const{
  return v * m;
}

Ray Transform::operator()(const Ray &r) const{
  auto newD = r.d * m;
  auto newO = r.o * m;
  return Ray(newD, newO);
}

Bounds3f Transform::operator()(const Bounds3f &b) const{
  auto newMinP = b.minPoint * m;
  auto newMaxP = b.maxPoint * m;
  return Bounds3f(newMinP, newMaxP);
}

Transform Transform::operator*(const Transform &t2) const{

}


}