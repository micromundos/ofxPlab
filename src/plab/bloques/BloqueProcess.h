#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

class BloqueProcess
{ 

  public:

    BloqueProcess() {};
    ~BloqueProcess() {}; 

    virtual void init(float proj_w, float proj_h) = 0;
    virtual void update(Bloque& bloque) = 0;
    virtual void render(Bloque& bloque) = 0;
    virtual string name() = 0;

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;
    }; 

    void inject(Fisica* fisica, Particles* particles) 
    {
      this->fisica = fisica;
      this->particles = particles;
    };

  protected:

    Fisica* fisica;
    Particles* particles;
};

