#ifndef SCENE_H
#define SCENE_H

#include "rt3.h"
#include "background.h"
#include "primitive.h"
#include "light.h"


namespace rt3{

class Scene{
public:
    unique_ptr<Background> background;
    unique_ptr<Primitive> primitive;
    vector<shared_ptr<Light>> lights;

    Scene(unique_ptr<Background> &&bg, unique_ptr<Primitive> &&prim):
        background(std::move(bg)), primitive(std::move(prim)){}

    ~Scene() = default;

    bool intersect(const Ray &r, shared_ptr<Surfel> &isect) const;
    bool intersect_p(const Ray &r) const;
};

} // namespace rt3


#endif