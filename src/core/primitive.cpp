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

    bool GeometricPrimitive::intersect(const Ray &r, unique_ptr<Surfel> &isect ) const{
        return shape->intersect(r, isect); 
    }

    bool PrimList::intersect(const Ray &r, unique_ptr<Surfel> &isect ) const{
        for(auto &prim : primitives)
        {
            if(prim->intersect(r, isect)){
                isect->setPrimitive(prim);
                return true;
            }
        }
        return false;
    }

}