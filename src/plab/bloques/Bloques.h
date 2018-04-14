#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"
#include "plab/bloques/BloqueProcess.h"

class Bloques
{ 
  public:

    Bloques() {};
    ~Bloques() 
    {
      dispose();
    }; 

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;
      _bloques = nullptr;

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
        vector<int> _ids = ids(procs[i]->name());
        procs[i]->init(_ids, proj_w, proj_h);
      }
    };

    void update(map<int, Bloque>& bloques)
    {
      this->_bloques = &bloques;
      for (int i = 0; i < procs.size(); i++)
      {
        vector<Bloque> proc_bloques = filter(procs[i]->name());
        procs[i]->update( proc_bloques );
      }
    };

    void render()
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<Bloque> proc_bloques = filter(procs[i]->name());
        procs[i]->render( proc_bloques );
      }
    };

    map<int, Bloque>& get()
    {
      return *_bloques;
    }; 

    vector<Bloque> filter(string name)
    { 
      vector<int> _ids = ids(name);
      vector<Bloque> filtered;
      for (auto& id : _ids)
        if (_bloques->find(id) != _bloques->end())
          filtered.push_back( (*_bloques)[id] );
      return filtered;
    };

  private:

    vector<shared_ptr<BloqueProcess>> procs;
    map<int, Bloque>* _bloques;

    Fisica* fisica;
    Particles* particles;
    ofxJSON plab_config;

    vector<int> ids(string name)
    {
      Json::Value cfg = plab_config["bloques"][name];
      vector<int> _ids;
      for (int i = 0; i < cfg.size(); i++)
        _ids.push_back( cfg[i].asInt() );
      return _ids;
    };
};

