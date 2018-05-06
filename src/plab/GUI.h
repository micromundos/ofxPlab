#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI
{
  public:

    ofParameter<bool> plab_monitor;
    ofParameter<bool> flowfield_debug;
    ofParameter<bool> syphon_projector;

    ofParameter<float> gaussian_kernel;
    ofParameter<float> gaussian_sigma;

    ofParameter<float> container_force_mult;
    //ofParameter<float> container_canny_threshold;
    ofParameter<float> container_edge_dilate;
    ofParameter<float> container_gaussian_kernel;
    ofParameter<float> container_gaussian_sigma; 

    ofParameter<float> attractors_force;
    ofParameter<float> attractors_radius;

    //ofParameter<float> transporter_force;
    //ofParameter<float> transporter_radius;

    void init_params()
    {
      p.add( plab_monitor
          .set("plab_monitor", true) );

      p.add( syphon_projector
          .set("syphon_projector", false) );

      p.add( gaussian_kernel
          .set("gaussian_kernel", 3., 0., 10.) );

      p.add( gaussian_sigma
          .set("gaussian_sigma", 5., 0., 20.) );

      p.add( flowfield_debug
          .set("flowfield_debug", true) );

      p.add( container_force_mult
          .set("container_force_mult", 1., 0., 10.) );

      //p.add( container_canny_threshold
          //.set("container_canny_threshold", 1., 0., 10.) );

      p.add( container_edge_dilate
          .set("container_edge_dilate", 1., 0., 10.) );

      p.add( container_gaussian_kernel
          .set("container_gaussian_kernel", 3., 0., 10.) );

      p.add( container_gaussian_sigma
          .set("container_gaussian_sigma", 5., 0., 20.) );

      p.add( attractors_force
          .set("attractors_force", 1., 0., 10.) );

      p.add( attractors_radius
          .set("attractors_radius", 0.17, 0., 1.) );

      //p.add( transporter_force
          //.set("transporter_force", 1., 0., 10.) );

      //p.add( transporter_radius
          //.set("transporter_radius", 0.1, 0., 1.) );
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

