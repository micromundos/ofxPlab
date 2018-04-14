#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

class FlowFieldTemplate : public FlowFieldLayer
{ 

  public:

    FlowFieldTemplate() {};
    ~FlowFieldTemplate() 
    {
      dispose();
    }; 

    ofTexture& get() 
    { 
      return proc.get(); 
    };

    float* get_data() 
    { 
      return proc.get_data();
    }; 

    void init(float w, float h, float proj_w, float proj_h) 
    {
      //FlowFieldLayer::init(w, h, proj_w, proj_h);
      //proc
        //.init("proc.frag", w, h)
        //.on("update", this, &FlowFieldTemplate::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      //proc
        //.off("update", this, &FlowFieldTemplate::update_proc)
        //.dispose();
    };

    void update(ofTexture& proj_tex, map<int, Bloque>& bloques)
    {
      //proc
        //.update()
        //.update_render(gui->plab_monitor);
    };

    void render()
    {};

    void render_monitor(float x, float& y, float w, float h)
    {
      if (!gui->plab_monitor)
        return;
    }; 

  private:

    gpgpu::Process proc;

    void update_proc(ofShader& shader)
    {
      shader.setUniform1f("template", 1.0);
    };
};

