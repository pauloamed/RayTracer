#include "directional.h"

namespace rt3{

tuple<Color, unique_ptr<VisibilityTester>, Vector3f> DirectionalLight::Li(const shared_ptr<Surfel>& hit){

    

    return tuple<Color, unique_ptr<VisibilityTester>, Vector3f>{
        color,
        make_unique<VisibilityTester>(VisibilityTester(hit, hit)),
        direction,
    };
}


}