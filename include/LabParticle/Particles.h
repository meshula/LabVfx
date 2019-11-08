
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#pragma once

#include "DataStripes.h"
#include <vector>

class Particles {
public:
    Particles();
    
    void update(float t, float dt);
    void respawn(const std::vector<int>& respawns, int count) { }
    
    const DataStripes& dataStripes() const { return _dataStripes; }
    DataStripes& dataStripes() { return _dataStripes; }
    
    void addEmitter(std::shared_ptr<Emitter> e) { _emitters.push_back(e); }
    void addIntegrator(std::shared_ptr<Integrator> i) { _integrators.push_back(i); }
    void addLifetimeRule(std::shared_ptr<LifetimeRule> r) { _lifeTimeRules.push_back(r); }
    void addRespawnParticles(std::shared_ptr<Particles> p) { _respawns.push_back(p); }
    void addLook(std::shared_ptr<Look> l) { _looks.push_back(l); }
    
private:
    DataStripes _dataStripes;
    std::vector<std::shared_ptr<Emitter>>      _emitters;
    std::vector<std::shared_ptr<Integrator>>   _integrators;
    std::vector<std::shared_ptr<LifetimeRule>> _lifeTimeRules;
    std::vector<std::shared_ptr<Particles>>    _respawns;
    std::vector<std::shared_ptr<Look>>         _looks;
    
    std::vector<int> _workArray;
};