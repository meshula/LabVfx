#ifndef LABVFX_EFFECT_H
#define LABVFX_EFFECT_H

#include "DataStripes.h"
#include <memory>
#include <vector>

/*

An effect has a list of simulation buckets, and a list of render passes.
Buckets appear once in the simulation list, as the effect will tick them all.
Buckets can appear as often as necessary in render passes.

lab::particle::Effect owns the buckets and passes, and will
delete them upon destruction. Buckets cannot be shared outside
of their owning Effect.

 */

namespace lab { namespace vfx
{
    class Bucket;
    class Emitter;
    class Field;
    class ForceIntegrator;
    class LifetimeRule;
    class Look;
    class Operator;

    enum class EffectRenderStyle
    {
        StyleInvisible,
        StyleStreak,
        StyleCameraFacing,
        StyleCameraFacingSingleColor,
        StyleSingleColorXZFlat
    };

    class EffectRenderPass
    {
    public:
        EffectRenderPass(
            Bucket* b, 
            EffectRenderStyle s, 
            float uniformScale,
            int skip, 
            void* quad_technique,
            void* line_technique)
        : _bucket(b), _style(s), _uniformScale(uniformScale)
        , _skip(skip)
        , _quad_technique(quad_technique), _line_technique(line_technique)
        { }

        Bucket* _bucket;
        EffectRenderStyle _style;
        float _uniformScale;
        int _skip;
        void* _quad_technique;
        void* _line_technique;
    };

    class Effect
    {
        float _current_time = 0;

    public:
        Effect();
        virtual ~Effect();

        void Restart();

        void update(float t, float dt);
        void respawn(const std::vector<int>& respawns, int count) { }
        
        std::shared_ptr<DataStripes> dataStripes() const { return _dataStripes; }
        
        void addOperator(std::shared_ptr<Operator> f) { _operators.push_back(f); }
        void addEmitter(std::shared_ptr<Emitter> e) { _emitters.push_back(e); }
        void addIntegrator(std::shared_ptr<ForceIntegrator> i) { _integrators.push_back(i); }
        void addLifetimeRule(std::shared_ptr<LifetimeRule> r) { _lifeTimeRules.push_back(r); }
        void addRespawnEffect(std::shared_ptr<Effect> p) { _respawns.push_back(p); }
        void addLook(std::shared_ptr<Look> l) { _looks.push_back(l); }
    
private:
        std::shared_ptr<DataStripes>               _dataStripes;
        std::vector<std::shared_ptr<Operator>>     _operators;
        std::vector<std::shared_ptr<Emitter>>      _emitters;
        std::vector<std::shared_ptr<ForceIntegrator>>   _integrators;
        std::vector<std::shared_ptr<LifetimeRule>> _lifeTimeRules;
        std::vector<std::shared_ptr<Effect>>       _respawns;
        std::vector<std::shared_ptr<Look>>         _looks;
        std::vector<EffectRenderPass>              _render_passes;
    
        std::vector<int> _workArray;
    };
}}

#endif
