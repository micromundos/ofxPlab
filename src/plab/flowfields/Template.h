#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

class Template : public FlowFieldLayer
{ 

  public:

    Template() {};
    ~Template() 
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

    void init(float ff_w, float ff_h, float proj_w, float proj_h) 
    {
      //FlowFieldLayer::init(ff_w, ff_h, proj_w, proj_h);
      //proc
        //.init("proc.frag", ff_w, ff_h)
        //.on("update", this, &Template::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      //proc
        //.off("update", this, &Template::update_proc)
        //.dispose();
    };

    void update(ofTexture& proj_tex)
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

