
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/Integrator.h"
#include "LabVfx/DataStripes.h"
#include "LabVfx/Effect.h"
#include "LabVfx/Field.h"

namespace lab { namespace vfx {

void Integrator::integrate(float t, float dt, Effect* system) 
{
    auto stripes = _stripes.lock();
    if (!stripes)
        return;

    size_t count = stripes->activeCount();
    if (!count)
        return;

    int stride = _pos->stride() / sizeof(float);
    int vstride = _vel->stride() / sizeof(float);
    float* force = _force->data<float>();
    float* pos = _pos->data<float>();
    float* vel = _vel->data<float>();
    const std::vector<int>& redirect = stripes->redirect();
    for (size_t i = 0; i < count; ++i) {
        int index = redirect[i] * 3;
        float fx = force[index + 0];
        float fy = force[index + 1];
        float fz = force[index + 2];
        float* p = &pos[index * stride];
        float* v = &vel[index * stride];
        v[0] += fx * dt;
        v[1] += fy * dt;
        v[2] += fz * dt;
        p[0] += v[0] * dt;
        p[1] += v[1] * dt;
        p[2] += v[2] * dt;
    }
}



}}
