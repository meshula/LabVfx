
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
    float* force = _force->data<float>(DataStripe::kFloat32_3);
    float* pos = _pos->data<float>(DataStripe::kFloat32_3);
    float* vel = _vel->data<float>(DataStripe::kFloat32_3);
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


void Field::setLocalToGlobal(const Imath::Matrix44<float>& g) 
{
    _globalTransform = g;
    std::error_code errc;
    _invGlobalTransform = g.inverse(errc);
    // check errc.value() == 0
}


void VelocityField::update(float t, float dt)
{
    auto stripes = _stripes.lock();
    if (!stripes)
        return;

    size_t count = stripes->activeCount();
    if (!count)
        return;

    Imath::Vec3<float> dpdt = _direction * _globalTransform;

    float* vel = _vel->data<float>(DataStripe::kFloat32_3);
    int stride = _vel->stride() / sizeof(float);
    const std::vector<int>& redirect = stripes->redirect();
    for (size_t i = 0; i < count; ++i) {
        int re = redirect[i] * stride;
        vel[re + 0] = dpdt.x;
        vel[re + 1] = dpdt.y;
        vel[re + 2] = dpdt.z;
    }
}

void AccelerationField::update(float t, float dt) 
{
    auto stripes = _stripes.lock();
    if (!stripes)
        return;

    size_t count = stripes->activeCount();
    if (!count)
        return;
 
    Imath::Vec3<float> dpdt = _direction * _globalTransform;

    float* force = _force_o_data->data<float>(DataStripe::kFloat32_3);
    int stride = _force_o_data->stride() / sizeof(float);
    const std::vector<int>& redirect = stripes->redirect();
    for (size_t i = 0; i < count; ++i) {
        int re = redirect[i] * stride;
        const float mass = 1.f; // mass could vary per particle
        Imath::Vec3<float> f = dpdt * mass;
        force[re + 0] = f.x;
        force[re + 1] = f.y;
        force[re + 2] = f.z;
    }
}

}}
