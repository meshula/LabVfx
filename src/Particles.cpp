
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "Particles.h"
#include "Emitter.h"
#include "Integrator.h"
#include "LifetimeRules.h"
#include "Looks.h"

Particles::Particles() { }

void Particles::update(float t, float dt)
{
    // run lifetime rules
    int respawn = 0;
    int c = _dataStripes.redirect().size();
    _workArray.resize(c);
    
    for (auto emitter : _emitters)
      emitter->emit(t, dt, this);
    
    for (auto integrator : _integrators)
        integrator->integrate(t, dt, this);
    
    for (auto life : _lifeTimeRules)
        for (int i = 0; i < c; ++i)
            if (life->expire(i))
                _workArray[respawn++] = i;
    
    for (auto look : _looks)
        look->apply(t, this);
    
    if (respawn) {
        // other systems may want to grab the expired particles
        for (auto r : _respawns)
            r->respawn(_workArray, respawn);
        
        _dataStripes.expire(_workArray, respawn);
    }
}
