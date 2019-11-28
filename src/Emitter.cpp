
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/Emitter.h"
#include "LabVfx/Effect.h"

#include <algorithm>
#include <math.h>

namespace lab { namespace vfx {

Emitter::Emitter()
: _emitterFunc(&emitOnPlane)
, _emitsPerSec(20)
, _residual(0)
, _emitted(0)
, _budget(SIZE_MAX)
, _rand(0) { }

void Emitter::Restart()
{
    _emitted = 0;
    _residual = 0;
    _rand.set_seed(0);
}

void Emitter::emit(float t, float dt, Effect* system) 
{
    if (!_emitterFunc || _emitted >= _budget)
        return;
 
    float request = _residual + dt * _emitsPerSec;
    size_t avail = system->dataStripes()->available();
    size_t genCount = std::min(static_cast<size_t>(request), avail);
    _residual += request - static_cast<float>(genCount);
    if (!genCount)
        return;
    
    std::shared_ptr<DataStripe> pos = system->dataStripes()->get("pos");
    uint8_t* data = pos->data<uint8_t>();
    // assert pos->data->kind == DataStripe::kFloat32_3;
    if (!data)
        return;
    size_t stride = pos->stride();

    std::shared_ptr<DataStripe> life = system->dataStripes()->get("life");
    uint8_t* life_data = life->data<uint8_t>();
    if (!life_data)
        return;
    size_t life_stride = life->stride();

    std::shared_ptr<DataStripe> vel = system->dataStripes()->get("vel");
    uint8_t* vel_data = vel->data<uint8_t>();
    // assert vel->data->kind == DataStripe::kFloat32_3;
    if (!vel_data)
        return;
    size_t vel_stride = vel->stride();

    const std::vector<int>& redirect = system->dataStripes()->redirect();
    size_t i = system->dataStripes()->activeCount();
    
    while (genCount > 0 && (_emitted < _budget))  {
        if (_budget < SIZE_MAX)
            ++_emitted;
        
        --genCount;
        size_t index = redirect[i];
        //printf("i %zd r[i] %zd\n", i, index);
        
        // really, the emitter function should bundle up "name", pointer, stride, type, and function
        float* p = reinterpret_cast<float*>(&data[index * stride]);
        _emitterFunc(p, _globalTransform, _rand);

        float* l = reinterpret_cast<float*>(&life_data[index * life_stride]);
        l[0] = _rand.nextf(0.5f, 2.f); // max life
        l[1] = 0.f;  // elapsed

        float* v = reinterpret_cast<float*>(&vel_data[index * vel_stride]);
        v[0] = 0.f;
        v[1] = 0.f;
        v[2] = 0.f;

        ++i;
    }
    
    system->dataStripes()->setActiveCount(i);
    _residual += genCount;
}

// static
void Emitter::emitOnPlane(float* xyzResult, const Imath::Matrix44<float>& transform, Random& rnd) 
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
