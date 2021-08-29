#include "sphere.h"

namespace rt3{
    
bool Sphere::intersect_p(const Ray &r, real_type maxT) const{
    auto invRay = transform->inverse().apply(r);

    Vector3f centerToOrigin = (r.o - origin);

    real_type A = r.d * r.d;
    real_type B = 2 * (centerToOrigin * r.d);
    real_type C = (centerToOrigin * centerToOrigin) - radius * radius;

    real_type delta = B * B - 4 * A * C;
    if(delta >= -0.0001){
        real_type t[2] = {
            (-B - sqrt(delta)) / (2 * A),
            (-B + sqrt(delta)) / (2 * A),
        };
        if(t[0] > t[1]) swap(t[0], t[1]);
        if(t[0] > 0) return t[0] < maxT;
        else if(t[1] > 0) return t[1] < maxT;
        else return false;
    }else return false;
}

bool Sphere::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
    auto invRay = transform->inverse().apply(r);

    Vector3f centerToOrigin = (r.o - origin);
    real_type A = r.d * r.d;
    real_type B = 2 * (centerToOrigin * r.d);
    real_type C = (centerToOrigin * centerToOrigin) - (radius * radius);
    real_type delta = (B * B) - (4 * A * C);

    if(delta >= -0.0001){
        real_type t[2] = {
            (-B - sqrt(delta)) / (2 * A),
            (-B + sqrt(delta)) / (2 * A),
        };
        if(t[0] > t[1]) swap(t[0], t[1]);

        Point3f contact;

        if(t[0] > 0) contact = r(t[0]);
        else if(t[1] > 0) contact = r(t[1]);
        else return false;

        Vector3f normal = (contact - origin).normalize();

        // O que devo transformar de volta?
        Point3f contactPoint = transform->apply(contact + normal * EPS);
        normal = transform->apply(normal);
        Vector3f newDir = transform->apply(r.d * -1);

        isect = unique_ptr<ObjSurfel>(new ObjSurfel(contactPoint, normal, newDir, t[0]));
        return true;
    }else{
        return false;
    }
}

Bounds3f Sphere::computeBounds() const{
  Point3f radiusPoint{{radius, radius, radius}};

  Point3f maxPoint = origin + radiusPoint;
  Point3f minPoint = origin + (radiusPoint * -1);
  return transform->apply(Bounds3f(minPoint, maxPoint));
}

Sphere *create_sphere(const ParamSet &ps){
    return new Sphere(
        retrieve(ps, "flip", false),
        retrieve(ps, "center", Point3f()),
        retrieve(ps, "radius", real_type(1))
    );
}

}