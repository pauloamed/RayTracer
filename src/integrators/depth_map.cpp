#include "depth_map.h"

namespace rt3{

Color DepthMapIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    unique_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return far_color;
    }else{
        real_type norm_t = normalizeT(isect->t);
        real_type norm_z = normalizeZ(norm_t);

        return clampColor(interpolate_color(norm_z, near_color, far_color));
    }
}

inline real_type DepthMapIntegrator::normalizeZ(real_type x) const{
    return (x - zmin) / z_range;
}

inline real_type DepthMapIntegrator::normalizeT(real_type x) const{
    return (x - scene_tmin) / t_range;
}

void DepthMapIntegrator::preprocess(const unique_ptr<Scene> &scene){
    auto w = camera->film->width(); // Retrieve the image dimensions in pixels.
    auto h = camera->film->height();

    for ( int i = 0 ; i < h; i++ ) {
        for( int j = 0 ; j < w ; j++ ) {        
            Ray ray = camera->generate_ray( i, j );

            unique_ptr<Surfel> isect; // Intersection information.  
            if (scene->intersect(ray, isect)) {
                scene_tmin = min(scene_tmin, isect->t);
                scene_tmax = max(scene_tmax, isect->t);
            }
        }
    }

    t_range = scene_tmax - scene_tmin;
}

void DepthMapIntegrator::render(const unique_ptr<Scene>& sc){
    preprocess(sc);
    SamplerIntegrator::render(sc);
}

DepthMapIntegrator* create_depth_map_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new DepthMapIntegrator(
        std::move(camera),
        retrieve(ps_integrator, "zmin", real_type(0)),
        retrieve(ps_integrator, "zmax", real_type(1)),
        retrieve(ps_integrator, "near_color", Color()),
        retrieve(ps_integrator, "far_color", Color())
    );
}

};