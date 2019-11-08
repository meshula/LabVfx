
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/LifetimeRules.h"
#include "LabVfx/DataStripes.h"

namespace lab { namespace vfx {

bool AgeRule::expire(float local_time, int i) const
{
    if (!_data)
        return false;

    float* data = _data->data<float>(DataStripe::Kind::kFloat32_1);
    float age = local_time - data[i];
    return age >= _max_age;
}

bool PlaneRule::expire(float local_time, int i) const
{
    if (!_data)
        return false;

    float* data = _data->data<float>(DataStripe::Kind::kFloat32_3);
    float pos = data[i*3 + _plane_index];
    return (pos < _height);
}

bool FireRule::expire(float local_time, int i) const
{
    if (!_heat_data)
        return false;

    float* data = _heat_data->data<float>(DataStripe::Kind::kFloat32_1);
    float temp = data[i];
    return (temp < _heat_threshold);
}

}};
