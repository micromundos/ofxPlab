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
        vector<int> ids;
        Json::Value ls = plab_config["bloques"][procs[i]->name()];
        for (int j = 0; j < ls.size(); j++)
          ids.push_back(ls[j].asInt());

        procs[i]->inject(fisica, particles);
        procs[i]->init(ids, proj_w, proj_h);
      }
    };

    void update(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<int> ids = procs[i]->get_ids();
        vector<Bloque> proc_bloques;
        for (auto& id : ids)
          if (bloques.find(id) != bloques.end())
            proc_bloques.push_back( bloques[id] );
        procs[i]->update( proc_bloques );
      }
    };

    void render(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<int> ids = procs[i]->get_ids();
        vector<Bloque> proc_bloques;
        for (auto& id : ids)
          if (bloques.find(id) != bloques.end())
            proc_bloques.push_back( bloques[id] );
        procs[i]->render( proc_bloques );
      }
    };

  private:

    vector<shared_ptr<BloqueProcess>> procs;

    Fisica* fisica;
    Particles* particles;
    ofxJSON plab_config;

};

