
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_EMITTER_H
#define LABVFX_EMITTER_H

#include "LabImath/ImathMatrix.h"
#include "LabVfx/Random.h"

namespace lab { namespace vfx {

class Effect;
class Random;

class Emitter {
public:
    Emitter();
    void emit(float t, float dt, Effect* system);
    
    static void emitOnPlane(float* xyzResult, const Imath::Matrix44<float>& transform, Random&);
    
    void setEmitsPerSec(float eps) { _emitsPerSec = eps; }
    void setBudget(int budget) { _budget = budget; }
    
    void setLocalToGlobal(const Imath::Matrix44<float>& g);

    void Restart();

private:
    Random _rand;
    Imath::Matrix44<float> _globalTransform;
    Imath::Matrix44<float> _invGlobalTransform;
    void (*_emitterFunc)(float*, const Imath::Matrix44<float>&, Random&);
    float _emitsPerSec;
    float _residual;
    int _emitted;
    int _budget;
};

}}

#endif
