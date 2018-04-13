#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/bloques/BloqueProcess.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

class Bloques
{ 
  public:

    Bloques() {};
    ~Bloques() 
    {
      dispose();
    };

    static vector<int> ids(Json::Value cfg)
    {
      vector<int> _ids;
      for (int i = 0; i < cfg.size(); i++)
        _ids.push_back( cfg[i].asInt() );
      return _ids;
    };

    static vector<Bloque> filter(map<int, Bloque>& bloques, Json::Value cfg)
    { 
      vector<int> ids = Bloques::ids(cfg);
      vector<Bloque> filtered;
      for (auto& id : ids)
        if (bloques.find(id) != bloques.end())
          filtered.push_back( bloques[id] );
      return filtered;
    }; 

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;

      for (int i = 0; i < procs.size(); i++)
        procs[i]->dispose();
      procs.clear(); //TODO will destroy shared_ptr's ???
    };

    void add(shared_ptr<BloqueProcess> bloque_proc)
    {
      procs.push_back(bloque_proc);
    };

    void inject(Fisica* fisica, Particles* particles, ofxJSON plab_config)
    {
      this->fisica = fisica;
      this->particles = particles;
      this->plab_config = plab_config;
    };

    void init(float proj_w, float proj_h)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        procs[i]->inject(fisica, particles, plab_config);
        vector<int> ids = Bloques::ids(plab_config["bloques"][procs[i]->name()]);
        procs[i]->init(ids, proj_w, proj_h);
      }
    };

    void update(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<Bloque> proc_bloques = Bloques::filter(bloques, plab_config["bloques"][procs[i]->name()]);
        procs[i]->update( proc_bloques );
      }
    };

    void render(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<Bloque> proc_bloques = Bloques::filter(bloques, plab_config["bloques"][procs[i]->name()]);
        procs[i]->render( proc_bloques );
      }
    };

  private:

    vector<shared_ptr<BloqueProcess>> procs;

    Fisica* fisica;
    Particles* particles;
    ofxJSON plab_config;

};

