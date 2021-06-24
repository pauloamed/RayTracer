#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "film.h"

namespace  rt3 {

    class Integrator
    {
        public:
            // Default init
            Integrator( void ){ }
            
            // Default copy assign
            Integrator& operator=( const Integrator& ) = default;
            
            // Default copy constructor
            Integrator( const Integrator& clone) = default;
            
            // Default destructor
            ~Integrator() = default;

            // render function. now taking film as parameter, but only temporarily
            void render( Film&, Background& );
    };


} // namespace rt3



#endif // INTEGRATOR_H