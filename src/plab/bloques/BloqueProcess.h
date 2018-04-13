#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

class BloqueProcess
{ 

  public:

    BloqueProcess() {};
    ~BloqueProcess() {};  

    virtual void update(vector<Bloque>& bloques) = 0;
    virtual void render(vector<Bloque>& bloques) = 0;
    virtual string name() = 0;

    virtual void init(vector<int> ids, float proj_w, float proj_h)
    {
      this->proj_w = proj_w;
      this->proj_h = proj_h;
    };

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;
    }; 

    void inject(Fisica* fisica, Particles* particles, ofxJSON plab_config) 
    {
      this->fisica = fisica;
      this->particles = particles;
      this->plab_config = plab_config;
    };

  protected:

    Fisica* fisica;
    Particles* particles;
    ofxJSON plab_config;

    float proj_w, proj_h;
};

