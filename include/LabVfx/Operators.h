
#ifndef LABVFX_OPERATORS_H
#define LABVFX_OPERATORS_H

#include <memory>

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;

class Operator {
public:
    Operator() { }
    virtual ~Operator() { }
    
    virtual void update(float t, float dt) = 0;
};

class Zero : public Operator {
public:
    explicit Zero(std::weak_ptr<DataStripes> s) : _stripes(s) { }
    virtual ~Zero() { }

    virtual void update(float t, float dt) override;

    void setZeroData(std::shared_ptr<DataStripe> s) { _stripe = s; }

private:
    std::shared_ptr<DataStripe> _stripe;
    std::weak_ptr<DataStripes> _stripes;
};

}}

#endif
