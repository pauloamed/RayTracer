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

    virtual bool intersect_p(const Ray &r, real_type maxT) const = 0;
    virtual bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const = 0;
};

} // namespace rt3










#endif