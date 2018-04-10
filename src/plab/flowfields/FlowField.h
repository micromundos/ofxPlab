#pragma once

#include "ofxCv.h"
#include "ofxGPGPU.h"

#include "ofxMicromundos/Bloque.h"

#include "plab/GUI.h"
#include "plab/flowfields/FlowFieldLayer.h"

class FlowField
{ 
  public:

    const int MAX_LAYERS = 4; //see flowfield_integration.frag

    FlowField() {};
    ~FlowField() 
    {
      dispose();
    }; 

    void add(shared_ptr<FlowFieldLayer> layer)
    {
      if (layers.size() < MAX_LAYERS)
        layers.push_back(layer); 
      else
        ofLogError("FlowField") << "does not support more than " << MAX_LAYERS << " layers";
    };

    void inject(shared_ptr<GUI> gui) 
    {
      this->gui = gui;
    };

    void init(float w, float h) 
    {
      ff = nullptr;
      ff_w = w;
      ff_h = h; 
      ff_chan = 4; //rgba TODO flowfield pass chan arg

      if (layers.size() == 0)
        ofLogWarning("FlowField") << "initialized with empty layers";

      for (int i = 0; i < layers.size(); i++)
      {
        layers[i]->inject(gui);
        layers[i]->init(w, h);
      }

      integration
        .init("glsl/flowfields/flowfield_integration.frag", w, h)
        .on("update", this, &FlowField::update_integration);
    };

    void dispose() 
    {
      ff = nullptr;
      gui = nullptr;

      for (int i = 0; i < layers.size(); i++)
        layers[i]->dispose();
      layers.clear(); //TODO will destroy shared_ptr's ???

      integration
        .off("update", this, &FlowField::update_integration)
        .dispose();

      input_pix.clear();
      input_tex.clear();
    };

    void update(ofTexture& tex, map<int, Bloque>& bloques)
    {
      if (!tex.isAllocated())
        return;
      input_tex = scale_tex(tex, ff_w, ff_h);
      update_ff(bloques);
    };

    void update(ofPixels& pix, map<int, Bloque>& bloques)
    {
      if (!pix.isAllocated())
        return;
      parse_input_pix(pix, input_pix, input_tex);
      update_ff(bloques);
    };

    void render(float x, float y, float w, float h)
    {
      integration.render(x, y, w, h);
    };

    void render_monitor(float x, float y, float w, float h)
    {
      if (!gui->plab_monitor)
        return;
      integration.render(x, y, w, h);
      y += h;
      for (int i = 0; i < layers.size(); i++)
        layers[i]->render_monitor(x, y, w, h);
    };

    void force_at(int x, int y, float& fx, float& fy)
    {
      int idx = (x + y * ff_w) * ff_chan;
      fx = ff[idx];
      fy = ff[idx+1];
    };

    float* get() { return ff; };
    float width() { return ff_w; };
    float height() { return ff_h; };
    float channels() { return ff_chan; };


  private:

    float* ff;
    float ff_w;
    float ff_h;
    float ff_chan;

    vector<shared_ptr<FlowFieldLayer>> layers;
    gpgpu::Process integration;

    ofTexture input_tex;
    ofFloatPixels input_pix;
    ofFbo scale_fbo;

    shared_ptr<GUI> gui;


    void update_ff(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < layers.size(); i++)
      {
        layers[i]->update(input_tex, bloques);
        integration.set("layer_"+ofToString(i), layers[i]->get());
      }

      integration
        .update()
        .update_render(gui->plab_monitor);

      ff = integration.get_data();
    };

    void update_integration(ofShader& shader)
    {
      shader.setUniform1i("n_layers", layers.size());
      //TODO flowfield integration weights
      float weight = 1.0/layers.size();
      for (int i = 0; i < layers.size(); i++)
        shader.setUniform1f("weight_"+ofToString(i), weight);
    };

    void parse_input_pix(ofPixels& src, ofFloatPixels& dst_pix, ofTexture& dst_tex)
    {
      float xscale = ff_w / src.getWidth();
      float yscale = ff_h / src.getHeight();

      ofPixels scaled;
      ofxCv::resize(src, scaled, xscale, yscale);

      if (!dst_pix.isAllocated())
        dst_pix.allocate(scaled.getWidth(), scaled.getHeight(), 4);

      for (int i = 0; i < scaled.size(); i++) 
      {
        unsigned char p = scaled[i];
        int j = i*4;
        dst_pix[j] = p/255.;
        dst_pix[j+1] = p/255.;
        dst_pix[j+2] = p/255.;
        dst_pix[j+3] = 1.;
      }

      dst_tex.loadData(dst_pix);
    }; 

    ofTexture scale_tex(ofTexture& src, int w, int h)
    {
      if (!scale_fbo.isAllocated())
      {
        ofFbo::Settings s;
        s.width = w;
        s.height = h;
        s.internalformat = GL_RGBA; //GL_RGBA32F_ARB; 
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.minFilter = GL_NEAREST;
        s.maxFilter = GL_NEAREST;
        s.wrapModeHorizontal = GL_CLAMP;
        s.wrapModeVertical = GL_CLAMP;
        scale_fbo.allocate(s); 
        //scale_fbo.allocate(w, h); 
      }

      scale_fbo.begin();
      ofClear(0,255);
      src.draw(0, 0, w, h);
      scale_fbo.end();

      return scale_fbo.getTexture(); //a copy
    };
};


