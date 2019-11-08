
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/Emitter.h"
#include "LabVfx/Effect.h"

#include <math.h>

namespace lab { namespace vfx {

Emitter::Emitter()
: _emitterFunc(&emitOnPlane)
, _emitsPerSec(20)
, _residual(0)
, _emitted(0)
, _budget(INT_MAX) { }

void Emitter::Restart()
{
    _emitted = 0;
    _residual = 0;
    _rand.init(0);
}

void Emitter::emit(float t, float dt, Effect* system) 
{
    if (!_emitterFunc || _emitted == _budget)
        return;
 
    float genCount = _residual + dt * _emitsPerSec;
    float avail = system->dataStripes().reserve(static_cast<int>(floorf(genCount)));
    if (avail < genCount) {
        _residual = genCount - avail;
        genCount = avail;
    }
    else
        _residual = 0;
    
    std::shared_ptr<DataStripe> pos = system->dataStripes().get("pos");
    const std::vector<int>& redirect = system->dataStripes().redirect();
    float* data = pos->data<float>(DataStripe::kFloat32_3);
    if (!data)
        return;
    
    int stride = pos->stride();
    int i = system->dataStripes().activeCount();
    
    while (genCount >= 1.0f && (_emitted < _budget)) {
        if (_budget < INT_MAX)
            ++_emitted;
        
        genCount -= 1.0f;
        int index = redirect[i];
        
        // really, the emitter function should bundle up "name", pointer, stride, type, and function
        float* p = &data[index * stride];
        _emitterFunc(p, _globalTransform, _rand);
        
        ++i;
    }
    
    system->dataStripes().setActiveCount(i);
    _residual += genCount;
}

// static
void Emitter::emitOnPlane(float* xyzResult, const Imath::Matrix44<float>& transform, Imath::Rand32& rnd) 
{
    Imath::Vec4<float> p { rnd.nextf(-0.5f, 0.5f), 0, rnd.nextf(-0.5f, 0.5f), 1 };
    p = p * transform;
    xyzResult[0] = p.x;
    xyzResult[1] = p.y;
    xyzResult[2] = p.z;
}

void Emitter::setLocalToGlobal(const Imath::Matrix44<float>& g) 
{
    _globalTransform = g;
    std::error_code err;
    _invGlobalTransform = g.inverse(err);
    // if (err.value() != 0) raiseError();
}


}}
