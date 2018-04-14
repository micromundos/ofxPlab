#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"
#include "plab/bloques/Bloques.h"

class Bloques;

class BloqueProcess
{ 

  public:

    BloqueProcess() {};
    ~BloqueProcess() {};  

    virtual void update() = 0;
    virtual void render() = 0;

    virtual void init(float proj_w, float proj_h)
    {
      this->proj_w = proj_w;
      this->proj_h = proj_h;
    };

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;
      bloques = nullptr;
    }; 

    void inject(Bloques* bloques, Fisica* fisica, Particles* particles) 
    {
      this->bloques = bloques;
      this->fisica = fisica;
      this->particles = particles;
    };

  protected:

    Fisica* fisica;
    Particles* particles;
    Bloques* bloques;

    float proj_w, proj_h;
};

