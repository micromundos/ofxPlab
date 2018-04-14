#pragma once

#include "ofxGPGPU.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

#define ATTRS_LEN 10
#define ATTRS_LOCS_LEN ATTRS_LEN*2

class FlowFieldAttractors : public FlowFieldLayer
{ 

  public:

    FlowFieldAttractors() {};
    ~FlowFieldAttractors() 
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
        .init("glsl/flowfields/flowfield_attractors.frag", w, h )
        .on("update", this, &FlowFieldAttractors::update_proc); 
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 
      proc
        .off("update", this, &FlowFieldAttractors::update_proc)
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
      vector<Bloque> attr_bloques = bloques->filter("attractor");
      ofPushStyle();
      ofNoFill();
      ofSetLineWidth(3);
      ofSetColor(ofColor::yellow);
      for (auto& b : attr_bloques)
        ofDrawCircle(
            b.loc.x * proj_w, 
            b.loc.y * proj_h, 
            radius_from_knob(b) * min(proj_w, proj_h));
      ofPopStyle();
    };

    void render_monitor(float x, float& y, float w, float h)
    {
      if (!gui->plab_monitor)
        return;
    }; 

  private:

    gpgpu::Process proc;

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

      vector<Bloque> attr_bloques = bloques->filter("attractor");

      int i = 0;
      for (auto& b : attr_bloques)
      {
        int ii = i*2; //b loc
        attractors_locs[ ii ] = b.loc.x;
        attractors_locs[ ii + 1 ] = b.loc.y;
        attractors_force[ i ] = gui->attractors_force;
        attractors_radius[ i ] = radius_from_knob(b);
        i++;
      }  

      shader.setUniform1i( "attractors_size", attr_bloques.size() );
      shader.setUniform2fv( "attractors_locs", attractors_locs, ATTRS_LEN );
      shader.setUniform1fv( "attractors_force", attractors_force, ATTRS_LEN );
      shader.setUniform1fv( "attractors_radius", attractors_radius, ATTRS_LEN );
    };

    float radius_from_knob(Bloque& b) 
    {
      //http://stackoverflow.com/questions/13097005/easing-functions-for-bell-curves
      float ang = b.angle_i / TWO_PI;
      float t = ((sin( TWO_PI * (ang - 0.25f)) + 1) * 0.5);
      float r = gui->attractors_radius;
      return ofMap(t, 0., 1., 0.01, r);
    };
};

