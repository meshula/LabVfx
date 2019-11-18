
#include "LabVfx/Operators.h"
#include "LabVfx/DataStripes.h"

namespace lab { namespace vfx {
    
void Zero::update(float t, float dt)
{
    if (!_stripe)
        return;

    auto stripes = _stripes.lock();
    if (!stripes)
        return;

    size_t count = stripes->activeCount();
    if (!count)
        return;

    int stride = _stripe->stride();

    uint8_t* data = _stripe->data<uint8_t>();
    memset(data, 0, stride * count);
}

}}