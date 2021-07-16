#ifndef SURFEL_H
#define SURFEL_H

#include "rt3.h"

namespace rt3
{
    struct Surfel{
        Point3f p;        //!< Contact point.
        Vector3f n;       //!< The surface normal.
        Vector3f wo;      //!< Outgoing direction of light, which is -ray.
        // Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
        shared_ptr<Primitive> primitive=nullptr; //!< Pointer to the primitive.

        Surfel(const Point3f&p, const Vector3f&n, const Vector3f&wo)
            : p{p}, n{n}, wo{wo} {};

        void setPrimitive(const shared_ptr<Primitive> &prim){
            primitive = prim;
        }
    };

} // namespace rt3






#endif