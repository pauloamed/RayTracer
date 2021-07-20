#include "integrator.h"
#include "material.h"

namespace rt3{

FlatIntegrator* create_flat_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new FlatIntegrator(std::move(camera));
}

NormalIntegrator* create_normal_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new NormalIntegrator(std::move(camera));
}

DepthMapIntegrator* create_depth_map_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new DepthMapIntegrator(
        std::move(camera),
        retrieve(ps_integrator, "zmin", real_type(0)),
        retrieve(ps_integrator, "zmmax", real_type(1)),
        retrieve(ps_integrator, "near_color", Color()),
        retrieve(ps_integrator, "far_color", Color())
    );
}


void SamplerIntegrator::render( const unique_ptr<Scene> &scene ) {
    // Perform objects initialization here.
    // The Film object holds the memory for the image.
    // ...
    auto w = camera->film->width(); // Retrieve the image dimensions in pixels.
    auto h = camera->film->height();
    // Traverse all pixels to shoot rays from.
    for ( int i = 0 ; i < h; i++ ) {
        for( int j = 0 ; j < w ; j++ ) {
            
            Ray ray = camera->generate_ray( i, j );
            auto backgroundColor = \
                scene->background->sampleXYZ( Point2f{
                    {float(i)/float(h),
                    float(j)/float(w)}
                } ); // get background color.
            
            
            Color pixelColor =  Li(ray, scene, backgroundColor);
         
            camera->film->add_sample( Point2i{{i,j}}, pixelColor ); // set image buffer at position (i,j), accordingly.
        }
    }
    // send image color buffer to the output file.
    camera->film->write_image();
}

Color FlatIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    unique_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        // Some form of determining the incoming radiance at the ray's origin.
        // For this integrator, it might just be:
        // Polymorphism in action.
        shared_ptr<FlatMaterial> fm = std::dynamic_pointer_cast<FlatMaterial>( isect->primitive->get_material() );
        // Assign diffuse color to L.
        return fm->getColor(); // Call a method present only in FlatMaterial.
    }
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
    // tmin;tmax correspondem a 0;1
    // zmin;zmax podem nao ser 0;1, mas 0;2
}

void DepthMapIntegrator::render(const unique_ptr<Scene>& sc){
    preprocess(sc);
    SamplerIntegrator::render(sc);
}

inline real_type DepthMapIntegrator::normalizeZ(real_type x) const{
    return (x - zmin) / z_range;
}

inline real_type DepthMapIntegrator::normalizeT(real_type x) const{
    return (x - scene_tmin) / t_range;
}

Color interpolate_color_(const float t, const Color &a, const Color &b){
  return Color{{
    (int) Lerp(t, a.x(), b.x()),
    (int) Lerp(t, a.y(), b.y()),
    (int) Lerp(t, a.z(), b.z()),
  }};
}

Color DepthMapIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    unique_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return far_color;
    }else{
        real_type norm_t = normalizeT(isect->t);
        if(norm_t < zmin || norm_t > zmax) return far_color;

        real_type norm_z = normalizeZ(norm_t);

        return interpolate_color_(norm_z, near_color, far_color);
    }
}




Color NormalIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    unique_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        return getColorFromNormal(isect->n);      
    }
}


int SamplerIntegrator::getColorFromCoord(real_type x) const{
    return x * 255;
}


Color NormalIntegrator::getColorFromNormal(const Vector3f &n) const{
    Vector3f inRange = (n + Vector3f({1, 1, 1})) * real_type(0.5);
    return Color({
        getColorFromCoord(Clamp<real_type, real_type, real_type>(inRange.at(0), 0.0, 1.0)),
        getColorFromCoord(Clamp<real_type, real_type, real_type>(inRange.at(1), 0.0, 1.0)),
        getColorFromCoord(Clamp<real_type, real_type, real_type>(inRange.at(2), 0.0, 1.0)),
    });
}


} // namespace rt3

