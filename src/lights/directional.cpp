#include "directional.h"

namespace rt3{

tuple<Color, unique_ptr<VisibilityTester>, Vector3f> DirectionalLight::Li(const shared_ptr<Surfel>& hit){

    return tuple<Color, unique_ptr<VisibilityTester>, Vector3f>{
        colorIntensity,
        make_unique<VisibilityTester>(VisibilityTester(hit, hit)),
        direction,
    };
}

DirectionalLight* create_directional_light( const ParamSet &ps ){
    Point3f from = retrieve(ps, "from", Point3f());
    Point3f to = retrieve(ps, "to", Point3f());

    return new DirectionalLight(
        retrieve(ps, "L", Color()),
        retrieve(ps, "scale", Vector3f()),
        (to - from)
    );
}

}