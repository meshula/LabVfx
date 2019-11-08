
#include "LabVfx/Effect.h"
#include "LabVfx/Emitter.h"
#include "LabVfx/Integrator.h"
#include "LabVfx/LifetimeRules.h"
#include "LabVfx/Looks.h"

namespace lab { namespace vfx {
    
    Effect::~Effect()
    {
    }

    void Effect::Restart()
    {
        _current_time = 0;
        for (auto emitter : _emitters)
            emitter->Restart();
    }

    void Effect::update(float t, float dt)
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
                if (life->expire(t, i))
                    _workArray[respawn++] = i;
        
        if (respawn) {
            // other systems may want to grab the expired particles
            for (auto r : _respawns)
                r->respawn(_workArray, respawn);
            
            _dataStripes.expire(_workArray, respawn);
        }

        // apply looks to particles that survived the cull
        for (auto look : _looks)
            look->apply(t, this);        
    }

}}
