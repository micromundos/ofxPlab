#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

class Transporter : public FlowFieldLayer
{ 

  public:

    Transporter() {};
    ~Transporter() 
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
        .init("glsl/flowfields/transporter.frag", ff_w, ff_h)
        .on("update", this, &Transporter::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      proc
        .off("update", this, &Transporter::update_proc)
        .dispose();
    };

    void update(ofTexture& proj_tex)
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
      vector<Bloque> _bloques = bloques->filter("transporter");
      shader.setUniform1i("transporters_size", _bloques.size());
      if (_bloques.size() < 2)
        return;
      shader.setUniform2f("src", _bloques[0].loc);
      shader.setUniform2f("dst", _bloques[1].loc);
      shader.setUniform1f("force", gui->transporter_force);
      shader.setUniform1f("radius", gui->transporter_radius);
    };
};

