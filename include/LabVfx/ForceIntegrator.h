
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_FORCEINTEGRATOR_H
#define LABVFX_FORCEINTEGRATOR_H

#include "Operator.h"

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;
class Effect;

class ForceIntegrator : public Operator {
public:
    explicit ForceIntegrator(std::shared_ptr<DataStripes> s) : Operator(s) {}
    virtual ~ForceIntegrator() = default;

    virtual void update(float t, float dt) override;
    
    void setForceIData(std::shared_ptr<DataStripe> force) { _force = force; }
    void setPositionIOData(std::shared_ptr<DataStripe> pos) { _pos = pos; }
    void setVelocityIOData(std::shared_ptr<DataStripe> vel) { _vel = vel; }
    void setMassIData(std::shared_ptr<DataStripe> mass) { _mass = mass; }
    void setChargeIData(std::shared_ptr<DataStripe> charge) { _charge = charge; }

private:
    std::shared_ptr<DataStripe> _force;
    std::shared_ptr<DataStripe> _pos;
    std::shared_ptr<DataStripe> _vel;
    std::shared_ptr<DataStripe> _mass;
    std::shared_ptr<DataStripe> _charge;
};


}}

#endif
