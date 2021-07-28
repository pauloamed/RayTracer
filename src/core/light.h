#ifndef LIGHT_H
#define LIGHT_H

#include "rt3-base.h"

namespace rt3{
// Verifica se há oclusão entre dois pontos de contato.
class VisibilityTester {
public:
    VisibilityTester()=default;
    VisibilityTester(const Surfel&, const Surfel&);
    bool unoccluded(const Scene& scene);
public:
    Surfel p0, p1;
};

class Light {
public:
    virtual ~Light(){};
    Light();
    /// Retorna a intensidade da luz, direção e o teste oclusão.
    virtual Color sample_Li(    const Surfel&   hit  /*in*/,
                                Vector3f        *wi  /*out*/,
                                VisibilityTester *vis /*out*/ ) = 0;
    virtual void preprocess( const Scene & ) {};
};

}



#endif