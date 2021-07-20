#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.h"
#include "camera.h"
#include "surfel.h"


namespace  rt3 {

class Integrator {
public:
    virtual ~Integrator(){};
    virtual void render( const unique_ptr<Scene>& ) = 0;
};


class SamplerIntegrator : public Integrator {
//=== Public interface
public:
    virtual ~SamplerIntegrator(){};
        // Default init    

    SamplerIntegrator( unique_ptr<Camera> &&_camera ){
        camera = std::move(_camera);
    }

    virtual Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const = 0;
    virtual void render( const unique_ptr<Scene>& );
    // virtual void preprocess( const unique_ptr<Scene>& );
    
protected:
    std::unique_ptr<Camera> camera;
};



class FlatIntegrator : public SamplerIntegrator {
public:
    ~FlatIntegrator(){};


    FlatIntegrator( unique_ptr<Camera> &&_camera ):
        SamplerIntegrator(std::move(_camera)){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
};


class NormalIntegrator : public SamplerIntegrator {
private:
    Color getColorFromNormal(const Vector3f &n) const;
    int getColorFromCoord(real_type x) const;
public:
    ~NormalIntegrator(){};


    NormalIntegrator( unique_ptr<Camera> &&_camera ):
        SamplerIntegrator(std::move(_camera)){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
};


class DepthMapIntegrator : public SamplerIntegrator {
private:
    real_type zmin, zmax;
    Color near_color, far_color;

    void preprocess(const unique_ptr<Scene>&);

public:
    ~DepthMapIntegrator(){};

    


    DepthMapIntegrator( unique_ptr<Camera> &&_camera, real_type z_min, real_type z_max, Color n_color, Color f_color ):
        SamplerIntegrator(std::move(_camera)), zmin(z_min), zmax(z_max), near_color(n_color), far_color(f_color){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
    void render( const unique_ptr<Scene>& ) override;
};



FlatIntegrator* create_flat_integrator(const ParamSet &, unique_ptr<Camera> &&);
NormalIntegrator* create_normal_integrator(const ParamSet &, unique_ptr<Camera> &&);
DepthMapIntegrator* create_depth_map_integrator(const ParamSet &, unique_ptr<Camera> &&);





} // namespace rt3



#endif // INTEGRATOR_H