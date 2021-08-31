#include "sphere.h"

namespace rt3{
    
real_type Sphere::bhaskara(const Ray &r, real_type &A) const{
    Vector3f centerToOrigin = (r.o - origin);
    A = r.d * r.d;
    real_type B = 2 * (centerToOrigin * r.d);
    real_type C = (centerToOrigin * centerToOrigin) - (radius * radius);
    real_type delta = (B * B) - (4 * A * C);

    return delta;
}


bool Sphere::getT(const Ray &r, real_type &t) const{
    
    real_type A;
    delta = bhaskara(r, A);
    if(delta >= -0.0001){
        real_type roots[2] = {
            (-B - sqrt(delta)) / (2 * A),
            (-B + sqrt(delta)) / (2 * A),
        };
        if(roots[0] > roots[1]) swap(roots[0], roots[1]);
        if(roots[0] < 0){
            if(roots[1] < 0) return false;
            else{
                t = roots[1];
                return true;
            }
        }else{
            t = roots[0];
            return true;
        }
    }else return false;
}


bool Sphere::intersect_p(const Ray &r, real_type maxT) const{
    auto invRay = transform->inverse().apply(r);
    real_type t;
    if(!getT(invRay, t)) return false;
    return t < maxT;
}


bool Sphere::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
    auto invRay = transform->inverse().apply(r);

    real_type t;
    if(!getT(invRay, t)) return false;
        
    Point3f contact = invRay(t);
    Vector3f normal = (contact - origin).normalize();

    isect = unique_ptr<ObjSurfel>(new ObjSurfel(
        transform->apply(contact + normal * EPS), // contact point
        transform->apply(normal), // normal
        transform->apply(invRay.d * -1), // inv ray dir
        t // t
    ));
    return true;
}

Bounds3f Sphere::computeBounds() const{
  Point3f radiusPoint{{radius, radius, radius}};

  Point3f maxPoint = origin + radiusPoint;
  Point3f minPoint = origin + (radiusPoint * -1);
  return transform->apply(Bounds3f(minPoint, maxPoint));
}

Sphere *create_sphere(const ParamSet &ps, shared_ptr<Transform> t){
    return new Sphere(
        retrieve(ps, "flip", false),
        retrieve(ps, "center", Point3f()),
        retrieve(ps, "radius", real_type(1)),
        t
    );
}

}