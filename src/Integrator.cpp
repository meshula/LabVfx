
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabParticle/Integrator.h"
#include "LabParticle/DataStripes.h"
#include "LabParticle/Effect.h"

namespace lab { namespace particle {

void Integrator::integrate(float t, float dt, Effect* system) 
{
    for (auto f : _fields)
        f->integrate(t, dt, system);
}


void Field::setLocalToGlobal(const Imath::Matrix44<float>& g) 
{
    _globalTransform = g;
    std::error_code errc;
    _invGlobalTransform = g.inverse(errc);
    // check errc.value() == 0
}


void UniformField::integrate(float t, float dt, Effect* system) 
{
    int count = system->dataStripes().activeCount();
    if (!count)
        return;

    std::shared_ptr<DataStripe> pos = system->dataStripes().get("pos");
    float* data = pos->data<float>(DataStripe::kFloat32_3);
    if (!data)
        return;

    Imath::Vec3<float> dpdt = _direction * dt * _globalTransform;

    int stride = pos->stride();
    
    const std::vector<int>& redirect = system->dataStripes().redirect();
    for (int i = 0; i < count; ++i) {
        int index = redirect[i];
        float* p = &data[index * stride];
        p[0] += dpdt.x;
        p[1] += dpdt.y;
        p[2] += dpdt.z;
    }
}

void AccelerationField::integrate(float t, float dt, Effect* system) 
{
    int count = system->dataStripes().activeCount();
    if (!count)
        return;

    std::shared_ptr<DataStripe> pos = system->dataStripes().get("pos");
    float* data = pos->data<float>(DataStripe::kFloat32_3);
    if (!data)
        return;
    std::shared_ptr<DataStripe> vel = system->dataStripes().get("vel");
    float* vdata = vel->data<float>(DataStripe::kFloat32_3);
    if (!vdata)
        return;
 
    Imath::Vec3<float> dpdt = _direction * dt * _globalTransform;

    const std::vector<int>& redirect = system->dataStripes().redirect();
    int stride = pos->stride();
    
    int vstride = vel->stride();
        
    for (int i = 0; i < count; ++i) {
        int index = redirect[i];
        float* p = &data[index * stride];
        float* v = &vdata[index * vstride];
        v[0] += dpdt.x;
        v[1] += dpdt.y;
        v[2] += dpdt.z;
        p[0] += v[0] * dt;
        p[1] += v[1] * dt;
        p[2] += v[2] * dt;
    }
}

}}
