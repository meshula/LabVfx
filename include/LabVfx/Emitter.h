
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_EMITTER_H
#define LABVFX_EMITTER_H

#include "Imath/ImathMatrix.h"
#include "Imath/ImathRandom.h"

namespace lab { namespace vfx {

class Effect;

class Emitter {
public:
    Emitter();
    void emit(float t, float dt, Effect* system);
    
    static void emitOnPlane(float* xyzResult, const Imath::Matrix44<float>& transform, Imath::Rand32&);
    
    void setEmitsPerSec(float eps) { _emitsPerSec = eps; }
    void setBudget(int budget) { _budget = budget; }
    
    void setLocalToGlobal(const Imath::Matrix44<float>& g);

    void Restart();

private:
    Imath::Rand32 _rand;
    Imath::Matrix44<float> _globalTransform;
    Imath::Matrix44<float> _invGlobalTransform;
    void (*_emitterFunc)(float*, const Imath::Matrix44<float>&, Imath::Rand32&);
    float _emitsPerSec;
    float _residual;
    int _emitted;
    int _budget;
};

}}

#endif
