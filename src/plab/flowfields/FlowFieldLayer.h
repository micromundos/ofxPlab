#pragma once

#include "plab/GUI.h"

class FlowFieldLayer
{ 

  public:

    FlowFieldLayer() {};
    ~FlowFieldLayer() {}; 

    virtual ofTexture& get() = 0;
    virtual float* get_data() = 0;
    virtual void init(float w, float h) = 0;
    virtual void update(ofTexture& proj_tex, map<int, Bloque>& bloques) = 0;
    virtual void render_monitor(float x, float& y, float w, float h) = 0;

    void dispose() 
    {
      gui = nullptr;
    };

    void inject(shared_ptr<GUI> gui, ofxJSON plab_config) 
    {
      this->gui = gui;
      this->plab_config = plab_config;
    };

  protected:

    shared_ptr<GUI> gui;
    ofxJSON plab_config;
};

