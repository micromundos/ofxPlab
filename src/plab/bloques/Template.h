#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/bloques/BloqueProcess.h"

class Template : public BloqueProcess
{ 
  public:

    Template() {};
    ~Template() 
    {
      dispose();
    };

    void dispose() 
    {
      BloqueProcess::dispose(); 
    }; 

    string name()
    {
      return "template";
    };

    void init(vector<int> ids, float proj_w, float proj_h) 
    {
      BloqueProcess::init(ids, proj_w, proj_h); 
    };

    void update(vector<Bloque>& bloques) 
    {
      for (auto& b : bloques)
        update(b);
    };

    void render(vector<Bloque>& bloques) 
    {
      for (auto& b : bloques)
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

