
#include "LabVfx/Field.h"
#include "LabVfx/DataStripes.h"

namespace lab { namespace vfx {
    

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

    float* vel = _vel->data<float>();
    size_t stride = _vel->stride() / sizeof(float);
    const std::vector<int>& redirect = stripes->redirect();

    if (_accumulate)
        for (size_t i = 0; i < count; ++i) {
            size_t re = redirect[i] * stride;
            vel[re + 0] += dpdt.x;
            vel[re + 1] += dpdt.y;
            vel[re + 2] += dpdt.z;
        }
    else
        for (size_t i = 0; i < count; ++i) {
            size_t re = redirect[i] * stride;
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

    float* force = _force_o_data->data<float>();
    size_t stride = _force_o_data->stride() / sizeof(float);
    const std::vector<int>& redirect = stripes->redirect();

    if (_accumulate)
        for (size_t i = 0; i < count; ++i) {
            size_t re = redirect[i] * stride;
            const float mass = 1.f; // mass could vary per particle
            Imath::Vec3<float> f = dpdt * mass;
            force[re + 0] = f.x;
            force[re + 1] = f.y;
            force[re + 2] = f.z;
        }
    else
        for (size_t i = 0; i < count; ++i) {
            size_t re = redirect[i] * stride;
            const float mass = 1.f; // mass could vary per particle
            Imath::Vec3<float> f = dpdt * mass;
            force[re + 0] += f.x;
            force[re + 1] += f.y;
            force[re + 2] += f.z;
        }
}

}}