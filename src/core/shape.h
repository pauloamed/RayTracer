#ifndef SHAPE_H
#define SHAPE_H

#include "basic_types.h"
#include "paramset.h"
#include "surfel.h"

namespace rt3{

class Shape{
public:
    bool flip_normals;

    ~Shape(){}

    virtual bool intersect_p(const Ray &r) const = 0;
    virtual bool intersect(const Ray &r, Surfel *isect) const = 0;
};

class Sphere : public Shape{
public:
    bool flip_normals;
    Point3f origin;
    real_type radius;
    
    Sphere(bool flip_n, Point3f ori, real_type r):
        flip_normals(flip_n), origin(ori), radius(r){}

    ~Sphere(){}

    bool intersect_p(const Ray &r) const override;
    bool intersect(const Ray &r, Surfel *isect) const override;
};


Sphere *create_sphere(const ParamSet &ps);

} // namespace rt3










#endif