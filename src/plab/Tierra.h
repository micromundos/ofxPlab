#pragma once

#include "plab/CoordMap.h"
#include "ofxMicromundos/utils.h"

class Tierra
{
  public:

    Tierra() {};
    ~Tierra() 
    {
      dispose();
    };

    void dispose() 
    {};

    void init(float proj_w, float proj_h)
    {
      this->proj_w = proj_w;
      this->proj_h = proj_h;
    };

    void update(ofTexture& proj_tex) 
    {
      tex_to_pix(proj_tex, proj_pix);
      screen_to_proj_pix.set(
          proj_w, 
          proj_h, 
          proj_pix.getWidth(), 
          proj_pix.getHeight());
    };

    void update(ofPixels& _proj_pix) 
    {
      ofxMicromundos::copy_pix(_proj_pix, proj_pix);
      screen_to_proj_pix.set(
          proj_w, 
          proj_h, 
          proj_pix.getWidth(), 
          proj_pix.getHeight());
    }; 

    int get(float screen_loc_x, float screen_loc_y)
    {
      ofVec2f proj_pix_loc;

      screen_to_proj_pix.dst(
        screen_loc_x, 
        screen_loc_y, 
        proj_pix_loc);

      ofColor c = proj_pix.getColor(
        proj_pix_loc.x, 
        proj_pix_loc.y);

      return c.getHex();
    };

    int get(ofVec2f& screen_loc)
    {
      return get(screen_loc.x, screen_loc.y);
    }; 

    bool is_tierra_firme(float screen_loc_x, float screen_loc_y)
    {
      return get(screen_loc_x, screen_loc_y) > 0;
    };

    bool is_tierra_firme(ofVec2f& screen_loc)
    {
      return is_tierra_firme(screen_loc.x, screen_loc.y);
    };

  private:

    ofPixels proj_pix;
    CoordMap screen_to_proj_pix;
    float proj_w, proj_h;

    //texture to pixels using fbo's readToPixels 
    //for openg gl es compatibility 🤞
    ofFbo _fbo;
    void tex_to_pix(ofTexture& src, ofPixels& dst)
    {
      if (!_fbo.isAllocated())
      {
        ofFbo::Settings s;
        s.width = src.getWidth();
        s.height = src.getHeight();
        s.internalformat = GL_RGBA; //GL_RGBA32F_ARB; 
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.minFilter = GL_NEAREST;
        s.maxFilter = GL_NEAREST;
        s.wrapModeHorizontal = GL_CLAMP;
        s.wrapModeVertical = GL_CLAMP;
        _fbo.allocate(s); 
      }

      _fbo.begin();
      ofClear(0, 255);
      src.draw(0, 0);
      _fbo.end();
      _fbo.readToPixels(dst, 0);
    };
};
