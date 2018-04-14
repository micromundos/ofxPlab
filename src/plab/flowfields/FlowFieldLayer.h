#pragma once

#include "plab/GUI.h"
#include "plab/bloques/Bloques.h"

class FlowFieldLayer
{ 

  public:

    FlowFieldLayer() {};
    ~FlowFieldLayer() {}; 

    virtual ofTexture& get() = 0;
    virtual float* get_data() = 0;

    virtual void update(ofTexture& proj_tex) = 0;
    virtual void render() = 0;
    virtual void render_monitor(float x, float& y, float w, float h) = 0;

    virtual void init(float ff_w, float ff_h, float proj_w, float proj_h)
    {
      this->proj_w = proj_w;
      this->proj_h = proj_h;
    };

    void dispose() 
    {
      gui = nullptr;
      bloques = nullptr;
    };

    void inject(shared_ptr<GUI> gui, Bloques* bloques) 
    {
      this->gui = gui;
      this->bloques = bloques;
    };

  protected:

    shared_ptr<GUI> gui;
    Bloques* bloques;

    float proj_w, proj_h;
};

