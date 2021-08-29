#ifndef SPHERE_H
#define SPHERE_H

#include "../core/shape.h"
#include "../core/transform.h"

namespace rt3{

class Sphere : public Shape{
public:
    bool flip_normals;
    Point3f origin;
    real_type radius;

    shared_ptr<Transform> transform;
    
    Sphere(bool flip_n, Point3f ori, real_type r, shared_ptr<Transform> t):
        flip_normals(flip_n), origin(ori), radius(r), transform(t){}

    ~Sphere(){}

    Bounds3f computeBounds() const override;
    bool intersect_p(const Ray &r, real_type maxT) const override;
    bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const override;
};


Sphere *create_sphere(const ParamSet &ps, shared_ptr<Transform> t);

}






#endif