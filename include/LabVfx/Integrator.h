
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_INTEGRATOR_H
#define LABVFX_INTEGRATOR_H

#include "Imath/ImathMatrix.h"
#include "Imath/ImathVec.h"

#include <memory>
#include <vector>

namespace lab { namespace vfx {

class Effect;

class Field {
public:
    Field() {
    }
    
    virtual void integrate(float t, float dt, Effect* system) = 0;
    
    void setLocalToGlobal(const Imath::Matrix44<float>& g);
    
protected:
    Imath::Matrix44<float> _globalTransform;
    Imath::Matrix44<float> _invGlobalTransform;
};


class UniformField : public Field {
public:
    
    UniformField() : _direction(0, 0, 1) { }
    virtual ~UniformField() { }
    
    virtual void integrate(float t, float dt, Effect* system) override;
    
private:
    Imath::Vec3<float> _direction;
};

class AccelerationField : public Field {
public:
    
    AccelerationField() : _direction(0, 0, 1) { }
    virtual ~AccelerationField() { }
    
    void setDirection(float x, float y, float z) {
        _direction.x = x;
        _direction.y = y;
        _direction.z = z;
    }
    
    virtual void integrate(float t, float dt, Effect* system) override;

private:
    Imath::Vec3<float> _direction;
};



class Integrator {
public:
    void integrate(float t, float dt, Effect* system);
    
    void addField(std::shared_ptr<Field> f) { _fields.push_back(f); }
    
private:
    std::vector<std::shared_ptr<Field>> _fields;
};


}}

#endif
