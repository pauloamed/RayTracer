#ifndef SHAPE_H
#define SHAPE_H

#include "basic_types.h"
#include "paramset.h"

namespace rt3{

class Shape{
public:
    bool flip_normals;

    // Shape(bool flip_n): flip_normals(flip_n){}
    // Shape(bool flip_n): flip_normals(flip_n){}
    ~Shape(){}

    virtual bool intersect_p(Ray r) = 0;
};

class Sphere : public Shape{
public:
    bool flip_normals;
    Point3f origin;
    real_type radius;
    
    Sphere(bool flip_n, Point3f ori, real_type r):
        flip_normals(flip_n), origin(ori), radius(r){}

    ~Sphere(){}

    bool intersect_p(Ray r) override;
};


Sphere *create_sphere(const ParamSet &ps);

} // namespace rt3










#endif