#include "primitive.h"

namespace rt3{

    bool PrimList::intersect_p(const Ray& r, real_type maxT) const{
        for(auto &prim : primitives)
        {
            if(prim->intersect_p(r, maxT)) return true;
        }
        return false;
    }

    bool GeometricPrimitive::intersect_p( const Ray& r, real_type maxT ) const{
        return shape->intersect_p(r, maxT); 
    }

    bool GeometricPrimitive::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect ) const{
        return shape->intersect(r, isect); 
    }

    bool PrimList::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect ) const{
        for(auto &prim : primitives)
        {   
            if(prim->intersect(r, isect)){
                isect->setPrimitive(std::dynamic_pointer_cast<GeometricPrimitive>(prim));
                return true;
            }
        }
        return false;
    }

}