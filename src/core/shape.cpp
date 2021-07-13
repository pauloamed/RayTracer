#include "shape.h"

namespace rt3{
    bool Sphere::intersect_p(Ray r){

        Vector3f centerToOrigin = (r.o - origin);

        std::stringstream ss; 
        ss << "ray dir " << r.d.toString() << "\n";
        ss << "ray ori " << r.o.toString() << "\n";
        ss << "ball ori " << origin.toString() << "\n";
        ss << "ball radius " << radius << "\n";
        ss << "center 2 origin " << centerToOrigin.toString() << "\n";
        ss << "center pop pooop " << (centerToOrigin * centerToOrigin) << "\n";
        // RT3_MESSAGE(ss.str());

        real_type A = r.d * r.d;
        real_type B = 2 * (centerToOrigin * r.d);
        real_type C = (centerToOrigin * centerToOrigin) - radius * radius;

        real_type delta = B * B - 4 * A * C;
        return delta >= -0.0001;
    }

    Sphere *create_sphere(const ParamSet &ps){
        return new Sphere(
            retrieve(ps, "flip", false),
            retrieve(ps, "center", Point3f()),
            retrieve(ps, "radius", real_type(1))
        );
    }

}