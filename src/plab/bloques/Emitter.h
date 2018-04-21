#pragma once

#include "ofxColorBrewer.h"

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
      int i = 0;
      int len = ids.size();

      vector<ofColor> cols = brewer
        //.get("Spectral", ofClamp(len, 1, 11));
        //.get("Dark2", ofClamp(len, 1, 7));
        //.get("Set1", ofClamp(len, 1, 6));
        //.get("YlGnBu", ofClamp(len, 1, 9));
        .get("YlOrRd", ofClamp(len, 1, 9));
        //.get("RdPu", ofClamp(len, 1, 9));

      for (auto& id : ids)
      {
        emit_remainder[id] = 0.;
        colors[id] = cols[i];
        i++;
      }

      emit_rate = 10.;
      emit_force = 15.;
    };

    void update() 
    {
      vector<Bloque> _bloques = bloques->filter("emitter");
      for (auto& b : _bloques)
        update(b);
    };

    void render() 
    {
      vector<Bloque> _bloques = bloques->filter("emitter");
      for (auto& b : _bloques)
        render(b);
    }; 

  private:

    map<int,float> emit_remainder;
    map<int,ofColor> colors;
    ofxColorBrewer brewer;

    float initial_fps;
    float emit_rate;
    float emit_force;
    float proj_diag;

    void update(Bloque& b) 
    {  
      // How many (fractional) particles should we have emitted this frame?
      float	dt = (1.0f / initial_fps);
      //float	dt = (1.0f / fisica->fps());
      //float	dt = (1.0f / ofGetFrameRate());
      emit_remainder[b.id] += emit_rate * dt;

      // Keep emitting particles on this frame until we only have a fractional particle left.
      while (emit_remainder[b.id] > 1.0f) 
      {
        emit_remainder[b.id] -= 1.0f;
        emit(b);
      }
    };

    void render(Bloque& b)
    {
      ofPushStyle();
      ofSetColor(colors[b.id]);
      ofFill();
      //ofSetLineWidth(4);
      ofVec2f p0(b.loc_i.x * proj_w, b.loc_i.y * proj_h);
      ofVec2f p1 = p0 + b.dir_i * offset(b) * 1.4;
      //ofDrawLine(p0.x, p0.y, p1.x, p1.y);
      ofVec2f p2 = p1.getRotated(-45, p0);
      p2 += (p0 - p2) * 0.5;
      ofVec2f p3 = p1.getRotated(45, p0);
      p3 += (p0 - p3) * 0.5;
      ofDrawTriangle(p1, p2, p3);
      ofPopStyle();
    };

    float offset(Bloque& b)
    {
      return b.radio * proj_diag;
    };

    void emit(Bloque& b)
    {
      b2ParticleSystem* b2ps = particles->b2_particles();

      ofVec2f proj_loc(b.loc.x * proj_w, b.loc.y * proj_h);
      proj_loc += b.dir * offset(b);

      int32 pidx = particles->make_particle(proj_loc.x, proj_loc.y, 0, 0, colors[b.id]);

      float force_m = emit_force;

      b2Vec2 force(b.dir.x, b.dir.y);
      force *= force_m;
      b2ps->ParticleApplyForce( pidx, force );
    };
};

