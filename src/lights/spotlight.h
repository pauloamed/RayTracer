#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "../core/light.h"

namespace rt3{

class SpotlightLight : public SamplerLight{
public:
    Point3f position;
    Vector3f lightDirection;
    real_type cutoff, falloff;
    real_type cosInterval;

    SpotlightLight(
        const Color &c, const Vector3f &scl, const Point3f &pos,
        const Vector3f& dir, real_type coff, real_type foff):
        SamplerLight(c, scl), position(pos), lightDirection(dir), cutoff(cos(Radians(coff))), falloff(cos(Radians(foff))){
            cosInterval = falloff - cutoff; 
        }

    
    tuple<Color, unique_ptr<VisibilityTester>, Vector3f> Li(const shared_ptr<Surfel>& hit) override;

};

SpotlightLight* create_spotlight_light( const ParamSet &ps );

}



#endif