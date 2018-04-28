#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/bloques/BloqueProcess.h"

class Portal : public BloqueProcess
{ 
  public:

    Portal() {};
    ~Portal() 
    {
      dispose();
    };

    void dispose() 
    {
      BloqueProcess::dispose(); 
    }; 

    void init(float proj_w, float proj_h) 
    {
      BloqueProcess::init(proj_w, proj_h); 
    };

    void update(vector<Bloque>& bloques) 
    {
      vector<Bloque> _bloques = bloques->filter("portal");
      for (auto& b : _bloques)
        update(b);
    };

    void render() 
    {
      vector<Bloque> _bloques = bloques->filter("portal");
      for (auto& b : _bloques)
        render(b);
    }; 

  private:

    void update(Bloque& bloque) 
    { 
    };

    void render(Bloque& bloque)
    {
    };
};

