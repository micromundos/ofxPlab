#pragma once

#include "ofxGPGPU.h"
#include "ofxGPGPU/shaders/gaussian.h"
#include "ofxCv.h"
#include "ofxMicromundos/Bloque.h"
#include "plab/flowfields/FlowFieldLayer.h"

class FlowFieldContainer : public FlowFieldLayer
{ 

  public:

    FlowFieldContainer() {};
    ~FlowFieldContainer() 
    {
      dispose();
    }; 

    ofTexture& get() 
    { 
      return container.get(); 
    };

    float* get_data() 
    { 
      return container.get_data();
    };

    void init(float w, float h, float proj_w, float proj_h) 
    {
      FlowFieldLayer::init(w, h, proj_w, proj_h);

      edges
        //.init("glsl/openvision/canny.fs", w, h)
        //.on("update",this,&FlowFieldContainer::update_canny); 
        .init("glsl/openvision/sobel.fs", w, h);

      dilate
        .add_backbuffer("tex")
        .init("glsl/openvision/dilate.fs", w, h);

      //erode
        //.add_backbuffer("tex")
        //.init("glsl/openvision/erode.fs", w, h);

      gaussian
        .init(w, h)
        .on("update",this,&FlowFieldContainer::update_gaussian);

      container
        .init("glsl/flowfields/flowfield_container.frag", w, h)
        .on("update",this,&FlowFieldContainer::update_container);
    };

    void dispose() 
    {
      FlowFieldLayer::dispose(); 

      edges
        //.off("update",this,&FlowFieldContainer::update_canny)
        .dispose();

      dilate.dispose();
      //erode.dispose();

      gaussian
        .off("update",this,&FlowFieldContainer::update_gaussian)
        .dispose();

      container
        .off("update",this,&FlowFieldContainer::update_container)
        .dispose();
    };

    void update(ofTexture& proj_tex)
    {
      //int canny_kernel = 3;
      //double canny_low_thres = 100;
      //double canny_high_thres = 150;
      //cv::Canny(src, edges_mat, canny_low_thres, canny_high_thres, canny_kernel);
      //ofxCv::invert(edges_mat);
      //cv::GaussianBlur(edges_mat, edges_mat, cv::Size(gui->gaussian_kernel, gui->gaussian_kernel), (double)gui->gaussian_sigma);

      edges
        .set("tex", proj_tex)
        .update();

      ofTexture* _edges;
      if (gui->edge_dilate > 0.)
      {
        dilate
          .set("tex", edges.get())
          .update(gui->edge_dilate);
        //erode
          //.set("tex", dilate.get())
          //.update(2);
        _edges = &(dilate.get());
      }
      else
        _edges = &(edges.get());

      if (gui->gaussian_kernel > 0.)
      {
        gaussian
          .set("data", *_edges)
          .update(2); //horiz + vert
        _edges = &(gaussian.get());
      }

      container
        .set("edges", *_edges)
        .update()
        .update_render(gui->plab_monitor);
    };

    void render()
    {};

    void render_monitor(float x, float& y, float w, float h)
    {
      if (!gui->plab_monitor)
        return;

      container.render(x, y, w, h);
      y += h; 

      if (gui->gaussian_kernel > 0.)
      {
        gaussian.get().draw(x, y, w, h);
        y += h;
      } 

      edges.get().draw(x, y, w, h);
      y += h;

      //toOf(edges_mat, edges_pix); 
      //if (edges_pix.isAllocated())
        //edges_tex.loadData(edges_pix);
      //if (edges_tex.isAllocated())
        //edges_tex.draw(x + w, y, w, h);
    }; 

  private:

    gpgpu::Process container;
    gpgpu::Process edges;
    gpgpu::Gaussian gaussian; 
    gpgpu::Process dilate;
    //gpgpu::Process erode;

    void update_container(ofShader& shader)
    {
      shader.setUniform1f("force_mult", gui->container_force_mult);
    };

    //void update_canny(ofShader& shader)
    //{
      //shader.setUniform1f("threshold", gui->canny_threshold);
    //};

    void update_gaussian(ofShader& shader)
    {
      shader.setUniform1f("sigma", gui->gaussian_sigma);
      shader.setUniform1i("kernel", gui->gaussian_kernel);
      shader.setUniform1f("alpha", 1.0);
    }; 
};


