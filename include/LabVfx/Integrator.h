
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_INTEGRATOR_H
#define LABVFX_INTEGRATOR_H

#include <memory>

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;
class Effect;

class Integrator {
public:
    explicit Integrator(std::weak_ptr<DataStripes> s) : _stripes(s) { }
    void integrate(float t, float dt, Effect* system);
    
    void setForceIData(std::shared_ptr<DataStripe> force) { _force = force; }
    void setPositionIOData(std::shared_ptr<DataStripe> pos) { _pos = pos; }
    void setVelocityIOData(std::shared_ptr<DataStripe> vel) { _vel = vel; }
    void setMassIData(std::shared_ptr<DataStripe> mass) { _mass = mass; }
    void setChargeIData(std::shared_ptr<DataStripe> charge) { _charge = charge; }

private:
    std::weak_ptr<DataStripes> _stripes;
    std::shared_ptr<DataStripe> _force;
    std::shared_ptr<DataStripe> _pos;
    std::shared_ptr<DataStripe> _vel;
    std::shared_ptr<DataStripe> _mass;
    std::shared_ptr<DataStripe> _charge;
};


}}

#endif
