#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

class FlowFieldTransporter : public FlowFieldLayer
{ 

  public:

    FlowFieldTransporter() {};
    ~FlowFieldTransporter() 
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
      FlowFieldLayer::init(ff_w, ff_h, proj_w, proj_h);
      proc
        .init("transporter.frag", ff_w, ff_h)
        .on("update", this, &FlowFieldTransporter::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      proc
        .off("update", this, &FlowFieldTransporter::update_proc)
        .dispose();
    };

    void update(ofTexture& proj_tex, map<int, Bloque>& bloques)
    {
      proc
        .update()
        .update_render(gui->plab_monitor);
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
      shader.setUniform1f("transporter", 1.0);
    };
};

