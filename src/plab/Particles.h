#pragma once

#include "plab/Fisica.h"
#include "plab/Tierra.h"
#include "plab/CoordMap.h"

class Particles
{ 

  public:

    Particles() {};
    ~Particles() 
    {
      dispose();
    };

    void inject(Fisica* fisica, Tierra* tierra) 
    {
      this->fisica = fisica;
      this->tierra = tierra;
    };

    void init(float proj_w, float proj_h) 
    {
      this->proj_w = proj_w;
      this->proj_h = proj_h;

      radius = 6.;
      max_particles = 5000.;
      max_speed = 3.;
      max_force = 0.;
      render_size = 4.;
      lifetime = 3.;
      lifetime_tierra = 60.;

      mesh.setMode(OF_PRIMITIVE_POINTS);

      //ofcolor.set( 127, 200, 255 );
      //b2color.Set(ofcolor.r, ofcolor.g, ofcolor.b, ofcolor.a);

      b2ParticleSystemDef psd;
      psd.radius = radius / fisica->scale();
      psd.maxCount = max_particles;

      /// Reduces relative velocity of viscous particles
      /// Larger values slow down viscous particles more
      psd.viscousStrength = 0.6f; //0.25f;

      /// Change the particle density.
      /// Particle density affects the mass of the particles, which in turn
      /// affects how the particles interact with b2Bodies. Note that the density
      /// does not affect how the particles interact with each other.
      //psd.density = 0.0f; //1.0f;

      /// Pushes particles out of solid particle group
      /// Larger values repulse more
      //psd.ejectionStrength = 0.0f; //0.5f;

      b2particles = fisica->b2world()->CreateParticleSystem(&psd);
    }; 

    void update(
        float* ff,
        float ff_w,
        float ff_h,
        float ff_chan)
    {
      update_lifetime();
      update_ff(ff, ff_w, ff_h, ff_chan);
      limit_speed();
    }; 

    void render()
    {
      mesh.clear();

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *locs = b2particles->GetPositionBuffer();
      b2ParticleColor *cols = b2particles->GetColorBuffer();

      ofVec2f loc;
      ofColor col;
      for (int i = 0; i < n; i++)
      {
        loc.set(locs[i].x, locs[i].y);
        col.set(cols[i].r, cols[i].g, cols[i].b);
        mesh.addVertex(loc);
        mesh.addColor(col);
      }

      ofPushStyle();
      ofSetColor(255);
      ofSetLineWidth(0.1);
      glPointSize(render_size);
      ofPushMatrix();
      ofScale(fisica->scale(), fisica->scale());
      mesh.draw();
      ofPopMatrix();
      ofPopStyle();
    };

    void dispose()
    {
      fisica = nullptr;
      tierra = nullptr;
      particles_in_tierra.clear();
    };

    int32 make_particle(float _locx, float _locy, float _velx, float _vely, ofColor _color)
    {
      float locx, locy, velx, vely;
      fisica->screen_to_world(_locx, _locy, locx, locy);
      fisica->screen_to_world(_velx, _vely, velx, vely);

      uint32 flags = b2_waterParticle | /*b2_springParticle |*/ b2_viscousParticle;

      b2ParticleDef pd;
      pd.flags = flags;
      pd.position.Set(locx, locy);
      pd.velocity.Set(velx, vely);
      b2ParticleColor p_color;
      p_color.Set(_color.r, _color.g, _color.b, 255.0);
      pd.color = p_color;
      pd.lifetime = lifetime; 

      int i = b2particles->CreateParticle(pd);
      const b2ParticleHandle* handle = b2particles->GetParticleHandleFromIndex(i);

      set<const b2ParticleHandle*>::iterator it = particles_in_tierra.find(handle);
      if (it != particles_in_tierra.end())
        particles_in_tierra.erase(it);

      return i;
    }; 

    b2ParticleSystem* b2_particles()
    {
      return b2particles;
    }; 

  private:

    Fisica* fisica;
    Tierra* tierra;

    b2ParticleSystem* b2particles;
    CoordMap screen_to_ff;

    ofVboMesh mesh;

    //ofColor ofcolor;
    //b2ParticleColor b2color;

    float proj_w, proj_h;

    float radius;
    float max_particles;
    float max_speed;
    float max_force;
    float render_size;

    float lifetime; 
    float lifetime_tierra;
    set<const b2ParticleHandle*> particles_in_tierra;

    void update_lifetime()
    {
      int32 n = b2particles->GetParticleCount();
      b2Vec2 *locs = b2particles->GetPositionBuffer(); 
      ofVec2f screen_loc;

      for (int i = 0; i < n; i++)
      {
        const b2ParticleHandle* handle = b2particles->GetParticleHandleFromIndex(i);

        b2Vec2& loc = locs[i]; 
        fisica->world_to_screen(loc, screen_loc);
        bool tierra_firme = tierra->is_tierra_firme(screen_loc);

        set<const b2ParticleHandle*>::iterator it = particles_in_tierra.find(handle);
        bool saved_in_tierra = it != particles_in_tierra.end();

        //enter tierra firme
        if (tierra_firme && !saved_in_tierra)
        {
          particles_in_tierra.insert(handle);
          b2particles->SetParticleLifetime(i, lifetime_tierra);
        }

        //leave tierra firme
        if (!tierra_firme && saved_in_tierra)
        {
          particles_in_tierra.erase(it);
          b2particles->SetParticleLifetime(i, lifetime);
        }
      }
    };

    void update_ff(
        float* ff,
        float ff_w,
        float ff_h,
        float ff_chan)
    {
      if (ff == nullptr) 
      {
        ofLogError() << "Particle::update flowfield is nullptr";
        return;
      }

      screen_to_ff.set(proj_w, proj_h, ff_w, ff_h);

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *locs = b2particles->GetPositionBuffer(); 

      b2Vec2 force;
      ofVec2f ff_loc, screen_loc;
      for (int i = 0; i < n; i++)
      {
        b2Vec2& loc = locs[i]; 

        fisica->world_to_screen(loc, screen_loc);
        screen_to_ff.dst(screen_loc, ff_loc);

        int idx = ((int)ff_loc.x + (int)ff_loc.y *ff_w) *ff_chan;
        float fx = ff[idx];
        float fy = ff[idx+1];
        force.Set(fx, fy);

        if (max_force > 0)
        {
          float len = force.Normalize();
          force *= len > max_force ? max_force : len;
        }

        b2particles->ParticleApplyForce(i, force);
      }
    };

    void limit_speed()
    {
      if (max_speed == 0.0)
        return;

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *vels = b2particles->GetVelocityBuffer(); 
      for (int i = 0; i < n; i++)
      {
        b2Vec2& vel = vels[i];
        float len = vel.Normalize();
        vel *= len > max_speed ? max_speed : len;
      }
    };
};

