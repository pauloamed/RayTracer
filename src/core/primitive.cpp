#include "primitive.h"

namespace rt3{

    bool PrimList::intersect_p(const Ray& r) const{
        for(auto &prim : primitives)
        {
            if(prim->intersect_p(r)) return true;
        }
        return false;
    }

    bool GeometricPrimitive::intersect_p( const Ray& r ) const{
        return shape->intersect_p(r); 
    }

    bool GeometricPrimitive::intersect(const Ray &r, Surfel *sf ) const{
        return false;
    }

    bool PrimList::intersect(const Ray &r, Surfel *sf ) const{
        return false;
    }

}