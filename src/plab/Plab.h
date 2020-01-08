#pragma once

#include "ofxJSON.h"

#include "plab/GUI.h"

#include "plab/Fisica.h"
#include "plab/Particles.h"

#include "plab/bloques/Bloques.h"
#include "plab/bloques/Emitter.h"
//#include "plab/bloques/Portal.h"

#include "plab/flowfields/FlowField.h"
#include "plab/flowfields/Container.h"
#include "plab/flowfields/Attractors.h"
//#include "plab/flowfields/Transporter.h"

class Plab
{

  public:

    Plab() {};
    ~Plab() 
    {
      dispose();
    };

    void inject(shared_ptr<GUI> gui, ofxJSON plab_config)
    {
      flowfield.inject(gui, &bloques);
      particles.inject(&fisica);
      bloques.inject(&fisica, &particles, plab_config);
      ff_w = plab_config["flow_field"]["width"].asFloat();
      ff_h = plab_config["flow_field"]["height"].asFloat();
    };

    void init(float proj_w, float proj_h)
    {
      fisica.init();
      particles.init(proj_w, proj_h);

      flowfield.add(make_shared<Container>());
      flowfield.add(make_shared<Attractors>("attractor", 1));
      flowfield.add(make_shared<Attractors>("repulsor", -1));
      //flowfield.add(make_shared<Transporter>());

      flowfield.init(ff_w, ff_h, proj_w, proj_h);

      bloques.add(make_shared<Emitter>());
      //bloques.add(make_shared<Portal>());
      bloques.init(proj_w, proj_h);
    };

    //ProjPix: ofPixels || ofTexture
    template <class ProjPix>
    void update(ProjPix& proj_pix, map<int, Bloque>& _bloques)
    {
      bloques.update(_bloques);
      flowfield.update(proj_pix);
      particles.update(proj_pix, flowfield.get(), flowfield.width(), flowfield.height(), flowfield.channels());
      fisica.update();
    };

    void render()
    {
      flowfield.render(); 
      bloques.render();
      particles.render();
    };

    void render_debug(float x, float y, float w, float h)
    {
      flowfield.render_debug(x, y, w, h);
    };

    void render_monitor(float x, float y, float w, float h)
    {
      flowfield.render_monitor(x, y, w, h);
    };

    void dispose()
    {};

  private:

    Fisica fisica;
    Particles particles;
    FlowField flowfield;
    Bloques bloques;

    float ff_w, ff_h;
};

