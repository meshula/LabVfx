
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/LifetimeRules.h"
#include "LabVfx/DataStripes.h"

namespace lab { namespace vfx {

void AgeRule::expire(float local_time, float dt) const
{
    if (!_age)
        return;

    auto stripes = _stripes.lock();
    if (!stripes)
        return;

    size_t c = stripes->activeCount();
    if (!c)
        return;
    
    size_t stride = _age->stride() / sizeof(float);
    float* age = _age->data<float>();
    const std::vector<int>& redirect = stripes->redirect();

    for (size_t i = 0; i < c; ++i) {
        int index = redirect[i];
        age[index * 2 + 1] += dt;
        if (age[index * 2 + 1] >= age[index * 2])
            stripes->expire(i);
    }
}

void PlaneRule::expire(float local_time, float t) const
{
    if (!_data)
        return;

    //float* data = _data->data<float>();
    //float pos = data[i*3 + _plane_index];
    //return (pos < _height);
}

void FireRule::expire(float local_time, float t) const
{
    if (!_heat_data)
        return;

    //float* data = _heat_data->data<float>();
    //float temp = data[i];
    //return (temp < _heat_threshold);
}

}};
