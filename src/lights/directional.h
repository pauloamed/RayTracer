#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include "../core/light.h"

namespace rt3{

class DirectionalLight : public SamplerLight{

virtual tuple<Color, unique_ptr<VisibilityTester>, Vector3f> Li(const shared_ptr<Surfel>& hit) override;

};



}



#endif