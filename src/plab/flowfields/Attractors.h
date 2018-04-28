#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

#define ATTRS_LEN 10
#define ATTRS_LOCS_LEN ATTRS_LEN*2

class Attractors : public FlowFieldLayer
{ 

  public:

    Attractors(string bloques_config_name, float force_multiplier) 
    {
      this->bloques_config_name = bloques_config_name;
      this->force_multiplier = force_multiplier;
    };

    ~Attractors() 
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
      FlowFieldLayer::init(w, h, proj_w, proj_h);
      proc
        .init("glsl/flowfields/attractors.frag", w, h )
        .on("update", this, &Attractors::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      proc
        .off("update", this, &Attractors::update_proc)
        .dispose();
    };

    void update(ofTexture& proj_tex)
    {
      proc
        .update()
        .update_render(gui->plab_monitor);
    };

    void render()
    {
      vector<Bloque> _bloques = bloques->filter(bloques_config_name);
      ofPushStyle();
      ofNoFill();
      ofSetLineWidth(3);
      for (auto& b : _bloques)
      {
        float k = knob(b);
        float r = attr_radius(k);
        ofSetColor(attr_color());
        ofDrawCircle(
            b.loc.x * proj_w, 
            b.loc.y * proj_h, 
            r * min(proj_w, proj_h));
      }
      ofPopStyle();
    };

    void render_monitor(float x, float& y, float w, float h)
    {
      if (!gui->plab_monitor)
        return;
    }; 

  private:

    gpgpu::Process proc;
    string bloques_config_name;
    float force_multiplier;

    //XXX WARNING keep in sync with ATTRS_LEN in fragment shader
    float attractors_locs[ATTRS_LOCS_LEN];
    float attractors_force[ATTRS_LEN];
    float attractors_radius[ATTRS_LEN];

    void update_proc(ofShader& shader)
    {
      for ( int i = 0; i < ATTRS_LOCS_LEN; i++ )
        attractors_locs[i] = 0.0;
      for ( int i = 0; i < ATTRS_LEN; i++ )
      {
        attractors_force[i] = 0.0;
        attractors_radius[i] = 0.0;
      }

      vector<Bloque> _bloques = bloques->filter(bloques_config_name);

      int i = 0;
      for (auto& b : _bloques)
      {
        int ii = i*2; //b loc
        float k = knob(b);
        attractors_locs[ ii ] = b.loc.x;
        attractors_locs[ ii + 1 ] = b.loc.y;
        attractors_force[ i ] = attr_force(k);
        attractors_radius[ i ] = attr_radius(k);
        i++;
      }  

      shader.setUniform1i("attractors_size", _bloques.size());
      shader.setUniform2fv("attractors_locs", attractors_locs, ATTRS_LEN);
      shader.setUniform1fv("attractors_force", attractors_force, ATTRS_LEN);
      shader.setUniform1fv("attractors_radius", attractors_radius, ATTRS_LEN);
    };

    //http://stackoverflow.com/questions/13097005/easing-functions-for-bell-curves
    float knob(Bloque& b) 
    {
      float ang = b.angle_i / TWO_PI; //[0,1]
      //return sin(TWO_PI*ang); //sin [-1,1] radius
      return (sin(TWO_PI*(ang-0.25f))+1)*0.5; //bell curve
    };

    float attr_force(float k)
    {
      return (k < 0. ? -1. : 1.) * gui->attractors_force * force_multiplier;
    };

    float attr_radius(float k) 
    {
      return ofMap(abs(k), 0., 1., 0.07, gui->attractors_radius);
      //return abs(k) * gui->attractors_radius;
    };

    ofColor attr_color() 
    {
      return force_multiplier < 0. 
        ? ofColor::fromHex(0xc51b8a) 
        : ofColor::fromHex(
            //0x41b6c4);
            //0x1d91c0);
            0x1c9099);
            //0x31a354);
    };
};

