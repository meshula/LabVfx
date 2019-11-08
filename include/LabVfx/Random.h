#ifndef LABVFX_RANDOM_H
#define LABVFX_RANDOM_H

namespace lab { namespace vfx {

class Random
{
    int64_t _seed;

public:
    explicit Random(int64_t seed) : _seed(seed) {}
    void set_seed(int64_t seed) { _seed = seed; }

    float nextf()
    {
        const int a = 1103515245;
        const int c = 12345;
        const int m = 2147483647;
        
        _seed = (_seed * a + c) & m;
        int64_t ret = _seed % 0x7fff;

        return (float)ret / (float) (0x7fff - 1);
    }

    float nextf(float min_, float max_)
    {
        return nextf() * (max_ - min_) + min_;
    }

};


}}

#endif
