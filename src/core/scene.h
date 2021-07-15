#ifndef SCENE_H
#define SCENE_H

#include "rt3.h"
#include "background.h"
#include "primitive.h"


namespace rt3{

class Scene{
public:
    unique_ptr<Background> background;
    unique_ptr<Primitive> primitive;

    Scene(unique_ptr<Background> &&bg, unique_ptr<Primitive> &&prim):
        background(std::move(bg)), primitive(std::move(prim)){}


    ~Scene() = default;
};

} // namespace rt3


#endif