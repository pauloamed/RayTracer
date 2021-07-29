#include "directional.h"

namespace rt3{

tuple<Color, unique_ptr<VisibilityTester>, Vector3f> DirectionalLight::Li(const shared_ptr<Surfel>& hit){

    Point3f virtualLightPoint = hit->p + (direction * -1 * mininumFreeDist);

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        virtualLightPoint,  // p
        direction,
        mininumFreeDist
    );

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, unique_ptr<VisibilityTester>, Vector3f>{
        colorIntensity,
        unique_ptr<VisibilityTester>(visTester),
        direction,
    };
}

DirectionalLight* create_directional_light( const ParamSet &ps ){
    Point3f from = retrieve(ps, "from", Point3f());
    Point3f to = retrieve(ps, "to", Point3f());

    return new DirectionalLight(
        retrieve(ps, "L", Color()),
        retrieve(ps, "scale", Vector3f()),
        (to - from).normalize()
    );
}

}