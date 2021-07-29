#include "blinn_phong.h"

#include "../lights/ambient.h"

namespace rt3{

Color BlinnPhongIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    shared_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        shared_ptr<Material> material = isect->primitive->get_material();

        Color color;

        for(auto &light : scene->lights){
            if(typeid(light) == typeid(AmbientLight)){
                
            }else{
                shared_ptr<SamplerLight> samplerLight = std::dynamic_pointer_cast<SamplerLight>(light);

                auto [lightColor, visTester, dir] = samplerLight->Li(isect);

                if(visTester->unoccluded(scene)){
                    // difuse
                    Color diffuseContrib = lightColor * max(real_type(0), isect->n * dir);
                    // specular
                    Color specularContrib = lightColor * fastExp(max(real_type(0), isect->n * dir), 1);

                    color = color + diffuseContrib + specularContrib;
                }
            }
        }
    }
}


BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet & ps_integrator, unique_ptr<Camera> &&camera, int depth){
    return new BlinnPhongIntegrator(
        std::move(camera),
        retrieve(ps_integrator, "depth", int(1))
    );
}


}