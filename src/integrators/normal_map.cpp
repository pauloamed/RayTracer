#include "normal_map.h"

namespace rt3{


Color NormalIntegrator::getColorFromNormal(const Vector3f &n) const{
    Vector3f inRange = (n + Vector3f({1, 1, 1})) * real_type(0.5);
    return clampColor(Color({
        getColorFromCoord(inRange.at(0)),
        getColorFromCoord(inRange.at(1)),
        getColorFromCoord(inRange.at(2)),
    }));
}

Color NormalIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    unique_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        return getColorFromNormal(isect->n);      
    }
}

NormalIntegrator* create_normal_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new NormalIntegrator(std::move(camera));
}

};