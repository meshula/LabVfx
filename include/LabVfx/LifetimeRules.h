
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABVFX_LIFETIMERULES_H
#define LABVFX_LIFETIMERULES_H

#include <memory>

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;

class LifetimeRule {
public:
    virtual void expire(float local_time, float dt) const = 0;
};

class AgeRule : public LifetimeRule
{
    std::weak_ptr<DataStripes> _stripes;
    std::shared_ptr<DataStripe> _age;

public:
    explicit AgeRule(std::weak_ptr<DataStripes> s) : _stripes(s) { }
    virtual ~AgeRule() = default;

    virtual void expire(float local_time, float dt) const override;

    void setAgeIOData(std::shared_ptr<DataStripe> age) { _age = age; }
};

class PlaneRule : public LifetimeRule
{
    DataStripe* _data;
    int _plane_index;
    float _height;

public:
    PlaneRule(DataStripe* position_data, int dimension_index, float height)
    : _data(position_data), _plane_index(dimension_index) { }

    virtual void expire(float local_time, float dt) const override;
};

class FireRule : public LifetimeRule
{
    DataStripe* _heat_data;
    float _heat_threshold;

public:
    FireRule(DataStripe* heat_data, DataStripe* fuel_data, float heat_threshold)
    : _heat_data(heat_data), _heat_threshold(heat_threshold) { }

    virtual void expire(float local_time, float dt) const override;
};

}}

#endif
