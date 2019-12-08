
#ifndef LABVFX_FIELD_H
#define LABVFX_FIELD_H

#include <LabImath/ImathMatrix.h>
#include <LabImath/ImathVec.h>
#include "Operator.h"

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;
class Effect;

class Field : public Operator {
public:
    explicit Field(std::weak_ptr<DataStripes> s, bool accumulate) 
        : Operator(s), _accumulate(accumulate) { }
    
    void setLocalToGlobal(const Imath::Matrix44<float>& g);

    // mass, charge
    // region, minimum radius of effect, strength, falloff
    
protected:
    Imath::Matrix44<float> _globalTransform;
    Imath::Matrix44<float> _invGlobalTransform;
    bool _accumulate;
};

// sets velocity to a value when a body enters it
class VelocityField : public Field {
public:
    VelocityField(std::weak_ptr<DataStripes> s, bool acc) 
        : Field(s, acc), _direction(0, 0, 1) { }

    virtual ~VelocityField() { }
    
    virtual void update(float t, float dt) override;
    void setVelocityIOData(std::shared_ptr<DataStripe> vel) { _vel = vel; }

private:
    Imath::Vec3<float> _direction;
    std::shared_ptr<DataStripe> _vel;
    std::weak_ptr<DataStripes> _stripes;
};

class AccelerationField : public Field {
public:
    explicit AccelerationField(std::weak_ptr<DataStripes> s, bool acc) 
             : Field(s, acc), _direction(0, 0, 1) { }
    virtual ~AccelerationField() { }
    
    void setDirection(float x, float y, float z) {
        _direction.x = x;
        _direction.y = y;
        _direction.z = z;
    }

    virtual void update(float t, float dt) override;

    void setForceOutOData(std::shared_ptr<DataStripe> o) { _force_o_data = o; }

private:
    Imath::Vec3<float> _direction;
    std::shared_ptr<DataStripe> _force_o_data;
};

//class DragField : public Field {};
//class ElectricField : public Field {};
//class LinearField : public Field {};
//class MagneticField : public Field {};
//class NoiseField : public Field {};
//class RadialGravityField : public Field {};
//class SpringField : public Field {};
//class TurbulenceField : public Field {};
//class DragField : public Field {};
//class VelocityTextureField : public Field {};
//class VortexField : public Field {};

}}

#endif
