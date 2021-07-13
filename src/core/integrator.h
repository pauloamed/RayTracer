#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.h"
#include "camera.h"

namespace  rt3 {

    class Integrator
    {
        public:

            unique_ptr<Camera> camera;

            // Default init
            Integrator( unique_ptr<Camera> &&_camera ){
                camera = std::move(_camera);
             }
            
            // Default copy assign
            Integrator& operator=( const Integrator& ) = default;
            
            // Default copy constructor
            Integrator( const Integrator& clone) = default;
            
            // Default destructor
            ~Integrator() = default;

            // render function. now taking film as parameter, but only temporarily
            void render( const unique_ptr<Scene>& );
    };


} // namespace rt3



#endif // INTEGRATOR_H