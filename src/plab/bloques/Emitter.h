#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/bloques/BloqueProcess.h"

class Emitter : public BloqueProcess
{ 
  public:

    Emitter() {};
    ~Emitter() 
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

      proj_diag = sqrt(proj_w*proj_w+proj_h*proj_h);
      initial_fps = fisica->fps();

      vector<int> ids = bloques->ids("emitter");
      for (auto& id : ids)
        emit_remainder[id] = 0.;

      emit_rate = 10.;
      emit_force = 15.;

      color = ofColor(255,255,255);
    };

    void update() 
    {
      vector<Bloque> _bloques = bloques->filter("emitter");
      for (auto& b : _bloques)
        update(b, emit_remainder[b.id]);
    };

    void render() 
    {
      vector<Bloque> _bloques = bloques->filter("emitter");
      for (auto& b : _bloques)
        render(b);
    }; 

  private:

    map<int,float> emit_remainder;
    float initial_fps;
    float emit_rate;
    float emit_force;
    ofColor color;
    float proj_diag;

    void update(Bloque& bloque, float& emit_remainder) 
    { 
      // How many (fractional) particles should we have emitted this frame?
      float	dt = (1.0f / initial_fps);
      //float	dt = (1.0f / fisica->fps());
      //float	dt = (1.0f / ofGetFrameRate());
      emit_remainder += emit_rate * dt;

      // Keep emitting particles on this frame until we only have a fractional particle left.
      while (emit_remainder > 1.0f) 
      {
        emit_remainder -= 1.0f;
        emit(bloque);
      }
    };

    void render(Bloque& bloque)
    {
      ofPushStyle();
      ofSetColor(ofColor::red);
      ofSetLineWidth(4);
      ofVec2f p0(bloque.loc_i.x*proj_w, bloque.loc_i.y*proj_h);
      ofVec2f p1 = p0 + bloque.dir_i * offset(bloque);
      ofDrawLine(p0.x, p0.y, p1.x, p1.y);
      ofPopStyle();
    };

    float offset(Bloque& b)
    {
      return b.radio * proj_diag;
      //ofVec2f loc(b.loc.x * proj_w, b.loc.y * proj_h);
      //ofVec2f corner(b.corners[0].x * proj_w, b.corners[0].y * proj_h);
      //return loc.distance(corner);
    };

    void emit(Bloque& bloque)
    {
      b2ParticleSystem* b2ps = particles->b2_particles();

      ofVec2f proj_loc(bloque.loc.x*proj_w, bloque.loc.y*proj_h);
      proj_loc += bloque.dir * offset(bloque);

      int32 pidx = particles->make_particle(proj_loc.x, proj_loc.y, 0, 0, color);

      float force_m = emit_force;

      b2Vec2 force( bloque.dir.x, bloque.dir.y );
      force *= force_m;
      b2ps->ParticleApplyForce( pidx, force );
    };
};

