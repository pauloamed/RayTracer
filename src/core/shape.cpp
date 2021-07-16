#include "shape.h"

namespace rt3{
    bool Sphere::intersect_p(const Ray &r) const{

        Vector3f centerToOrigin = (r.o - origin);

        real_type A = r.d * r.d;
        real_type B = 2 * (centerToOrigin * r.d);
        real_type C = (centerToOrigin * centerToOrigin) - radius * radius;

        real_type delta = B * B - 4 * A * C;
        return delta >= -0.0001;
    }

    bool Sphere::intersect(const Ray &r, Surfel *isect) const{
        Vector3f centerToOrigin = (r.o - origin);
        real_type A = r.d * r.d;
        real_type B = 2 * (centerToOrigin * r.d);
        real_type C = (centerToOrigin * centerToOrigin) - radius * radius;
        real_type delta = B * B - 4 * A * C;

        // isect = new Surfel();
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