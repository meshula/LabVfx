
#ifndef LABVFX_OPERATOR_H
#define LABVFX_OPERATOR_H

#include <memory>

namespace lab { namespace vfx {

class DataStripe;
class DataStripes;

class Operator {
protected:
    std::weak_ptr<DataStripes> _stripes;

public:
    Operator(std::weak_ptr<DataStripes> s) : _stripes(s) { }
    virtual ~Operator() = default;
    
    virtual void update(float t, float dt) = 0;
};

class Zero : public Operator {
public:
    explicit Zero() = default;
    virtual ~Zero() = default;

    virtual void update(float t, float dt) override;

    void setZeroData(std::shared_ptr<DataStripe> s) { _stripe = s; }

private:
    std::shared_ptr<DataStripe> _stripe;
};

}}

#endif
