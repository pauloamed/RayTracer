#include "blinn_phong.h"

#include "../lights/ambient.h"
#include "../materials/blinn_phong.h"

namespace rt3{

Vector3f computeHalfVector(const Vector3f &viewDir, const Vector3f &lightDir){
    auto h = viewDir + lightDir;
    return h.normalize();
}

Color BlinnPhongIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    shared_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        shared_ptr<BlinnPhongMaterial> material = \
            std::dynamic_pointer_cast<BlinnPhongMaterial>(isect->primitive->get_material());

        Color color;

        for(auto &light : scene->lights){
            if(typeid(light) == typeid(AmbientLight)){
                // TODO AMBIENT LIGHT
            }else{
                shared_ptr<SamplerLight> samplerLight = std::dynamic_pointer_cast<SamplerLight>(light);

                auto [lightColor, visTester, lightDir] = samplerLight->Li(isect);

                if(visTester->unoccluded(scene)){
                    // difuse
                    {
                        real_type coef = max(real_type(0), isect->n * lightDir);
                        Color diffuseContrib = material->diffuse * lightColor * coef;
                        
                        color = color + diffuseContrib;
                    }
                    
                    // specular
                    {
                        auto h = computeHalfVector(ray.d, lightDir);

                        real_type coef = max(real_type(0), isect->n * h);
                        coef = fastExp(coef, material->glossiness);
                        Color specularContrib = material->specular * lightColor * coef;
                    
                        color = color + specularContrib;
                    }
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