#include "scene.h"

namespace rt3{
    bool Scene::intersect(const Ray &r, unique_ptr<Surfel> &isect) const{
        return primitive->intersect(r, isect);
    }

    bool Scene::intersect_p(const Ray &r) const{
        return primitive->intersect_p(r);
    }

}