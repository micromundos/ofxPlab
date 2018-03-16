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
      this->ids = ids;
      this->proj_w = proj_w;
      this->proj_h = proj_h;
    };

    vector<int> get_ids()
    {
      return ids;
    };

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

    vector<int> ids;
    float proj_w, proj_h;
};

