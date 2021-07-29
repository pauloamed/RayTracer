#ifndef LIGHT_H
#define LIGHT_H

#include "rt3-base.h"

namespace rt3{
// Verifica se há oclusão entre dois pontos de contato.
class VisibilityTester {
public:
    shared_ptr<Surfel> objectContact, lightContact;
    VisibilityTester()=default;

    // eh consturido com os dois surfel
    VisibilityTester(const shared_ptr<Surfel>& obj, const shared_ptr<Surfel>& light):
        objectContact(obj), lightContact(light){}

    // vai iterar por todos objs ta cena vendo se tem contato
    bool unoccluded(const unique_ptr<Scene>& scene);    
};

class Light {
public:
    virtual ~Light(){};
    Light();
    virtual void preprocess( const Scene & ) {};

    Color color;
};

class SamplerLight : public Light {
public:
    virtual ~SamplerLight(){};
    SamplerLight();
    /// Retorna a intensidade da luz, direção e o teste oclusão.
    // funcao em que vai ser passado o surfel que queremos saber se esta iluminado
    // o surfel diz o ponto 3d P
    // de P, vamos atirar um ray pra achar o ponto de interseção Q com a luz
    // o visibility tester vai receber P e Q e ver se tem oclusão
    virtual tuple<Color, unique_ptr<VisibilityTester>, Vector3f> Li(const shared_ptr<Surfel>& hit) = 0;
    virtual void preprocess( const Scene & ) {};
};



}



#endif