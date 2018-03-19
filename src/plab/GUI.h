#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI
{
  public:

    ofParameter<bool> plab_monitor;
    ofParameter<bool> backend_debug_pixels;
    ofParameter<bool> flowfield_debug;
    ofParameter<bool> send_syphon;

    ofParameter<float> container_force_mult;
    //ofParameter<float> canny_threshold;
    ofParameter<float> edge_dilate;
    ofParameter<float> gaussian_kernel;
    ofParameter<float> gaussian_sigma;

    void init_params()
    {
      p.add( plab_monitor
          .set("plab_monitor", true) );

      p.add( send_syphon
          .set("send_syphon", false) );

      p.add( backend_debug_pixels
          .set("backend_debug_pixels", true) );

      p.add( flowfield_debug
          .set("flowfield_debug", true) );

      p.add( container_force_mult
          .set("container_force_mult", 1., 0., 10.) );

      //p.add( canny_threshold
          //.set("canny_threshold", 1., 0., 10.) );

      p.add( edge_dilate
          .set("edge_dilate", 1., 0., 10.) );

      p.add( gaussian_kernel
          .set("gaussian_kernel", 3., 0., 10.) );

      p.add( gaussian_sigma
          .set("gaussian_sigma", 5., 0., 20.) );
    };

    void init(float w)
    {
      ofxBaseGui::setDefaultWidth(w);
      p.setName("params");
      init_params();
      gui.setup(p, "plab_gui.xml");
      gui.loadFromFile("plab_gui.xml");
    };

    void render(float x, float y)
    {
      gui.setPosition(x, y);
      gui.draw();
    }; 

  private:

    ofParameterGroup p;
    ofxPanel gui; 
};

