#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3-base.h"


namespace rt3{

class Material{
public:
    Material() = default;
    virtual ~Material() = default;
};

class FlatMaterial : public Material{
public:
    Color color;
    FlatMaterial(Color c):color(c){}
    Color getColor() const{ return color; }
};


FlatMaterial * create_flat_material( const ParamSet &ps );
    
}; // namespace rt3





#endif