#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include "../core/light.h"

namespace rt3{

class DirectionalLight : public Light{

Color sample_Li(const Surfel& hit, Vector3f *wi, VisibilityTester *vis) override;

};



}



#endif