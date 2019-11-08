
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#ifndef LABPARTICLE_LIFETIMERULES_H
#define LABPARTICLE_LIFETIMERULES_H

namespace lab { namespace particle {

class DataStripe;

class LifetimeRule {
public:
    virtual bool expire(float local_time, int i) const { return false; }
};

class AgeRule : public LifetimeRule
{
    DataStripe* _data;
    float _max_age;

public:
    AgeRule(DataStripe* birth_time_data, float age)
    : _data(birth_time_data), _max_age(age) { }

    virtual bool expire(float local_time, int i) const override;
};

class PlaneRule : public LifetimeRule
{
    DataStripe* _data;
    int _plane_index;
    float _height;

public:
    PlaneRule(DataStripe* position_data, int dimension_index, float height)
    : _data(position_data), _plane_index(dimension_index) { }

    virtual bool expire(float local_time, int i) const override;
};

class FireRule : public LifetimeRule
{
    DataStripe* _heat_data;
    float _heat_threshold;

public:
    FireRule(DataStripe* heat_data, DataStripe* fuel_data, float heat_threshold)
    : _heat_data(heat_data), _heat_threshold(heat_threshold) { }

    virtual bool expire(float local_time, int i) const override;
};

}}

#endif
