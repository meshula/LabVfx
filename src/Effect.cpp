
#include "LabVfx/Effect.h"
#include "LabVfx/Emitter.h"
#include "LabVfx/Integrator.h"
#include "LabVfx/LifetimeRules.h"
#include "LabVfx/Looks.h"
#include "LabVfx/Operator.h"

namespace lab { namespace vfx {
    
    Effect::Effect()
    : _dataStripes(std::make_shared<DataStripes>())
    {
    }

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
        _current_time += dt;

        // run lifetime rules
        int respawn = 0;
        size_t c = _dataStripes->stripe_capacity();
        _workArray.clear();
        _workArray.reserve(c);
        
        for (auto& emitter : _emitters)
          emitter->emit(t, dt, this);

        for (auto& op : _operators)
            op->update(t, dt);

        for (auto& integrator : _integrators)
            integrator->integrate(t, dt, this);
        
        for (auto& life : _lifeTimeRules)
            life->expire(t, dt);
        
        if (respawn) {
            // other systems may want to grab the expired particles
            for (auto r : _respawns)
                r->respawn(_workArray, respawn);
            
            _dataStripes->expire(_workArray, respawn);
        }

        // apply looks to particles that survived the cull
        for (auto look : _looks)
            look->apply(t, this);        
    }

}}
