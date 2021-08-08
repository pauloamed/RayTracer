#ifndef SPHERE_H
#define SPHERE_H

#include "../core/shape.h"

namespace rt3{

class Sphere : public Shape{
public:
    bool flip_normals;
    Point3f origin;
    real_type radius;
    
    Sphere(bool flip_n, Point3f ori, real_type r):
        flip_normals(flip_n), origin(ori), radius(r){}

    ~Sphere(){}

    bool intersect_p(const Ray &r, real_type maxT) const override;
    bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const override;
};


Sphere *create_sphere(const ParamSet &ps);

}






#endif