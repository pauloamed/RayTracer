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

    bool Sphere::intersect(const Ray &r, unique_ptr<Surfel> &isect) const{
        Vector3f centerToOrigin = (r.o - origin);
        real_type A = r.d * r.d;
        real_type B = 2 * (centerToOrigin * r.d);
        real_type C = (centerToOrigin * centerToOrigin) - (radius * radius);
        real_type delta = (B * B) - (4 * A * C);

        if(delta >= -0.0001){
            real_type t[2] = {
                (-B - sqrt(delta)) / (2 * A),
                (-B + sqrt(delta)) / (2 * A),
            };
            if(t[0] > t[1]) swap(t[0], t[1]);

            Point3f contact = r(t[0]);

            Vector3f normal = (contact - origin).normalize();

            isect = unique_ptr<Surfel>(new Surfel(contact, normal, r.d * -1, t[0]));
            return true;
        }else{
            return false;
        }
    }

    Sphere *create_sphere(const ParamSet &ps){
        return new Sphere(
            retrieve(ps, "flip", false),
            retrieve(ps, "center", Point3f()),
            retrieve(ps, "radius", real_type(1))
        );
    }

}