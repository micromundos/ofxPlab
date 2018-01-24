#pragma once

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

      proc_tags.clear();
    };

    void add(shared_ptr<BloqueProcess> bloque_proc)
    {
      procs.push_back(bloque_proc);
    };

    void inject(Fisica* fisica, Particles* particles, cv::FileStorage plab_config)
    {
      this->fisica = fisica;
      this->particles = particles;
      this->plab_config = plab_config;
    };

    void init(float proj_w, float proj_h)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        procs[i]->inject(fisica, particles);
        procs[i]->init(proj_w, proj_h);

        string name = procs[i]->name();
        vector<int> ids;
        plab_config["bloques"][name] >> ids;
        proc_tags[name] = ids;
      }
    };

    void update(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<int> ids = proc_tags[procs[i]->name()];
        for (auto& id : ids)
          if (bloques.find(id) != bloques.end())
            procs[i]->update( bloques[id] );
      }
    };

    void render(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
      {
        vector<int> ids = proc_tags[procs[i]->name()];
        for (auto& id : ids)
          if (bloques.find(id) != bloques.end())
            procs[i]->render( bloques[id] );
      }
    };

  private:

    vector<shared_ptr<BloqueProcess>> procs;
    map<string, vector<int>> proc_tags;

    Fisica* fisica;
    Particles* particles;
    cv::FileStorage plab_config;

};

