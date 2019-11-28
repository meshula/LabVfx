#ifndef LABVFX_RANDOM_H
#define LABVFX_RANDOM_H

namespace lab { namespace vfx {

// xorshift from
// https://github.com/AndreasMadsen/xorshift/blob/master/reference.c
// MIT license

class Random
{
    /* The state must be seeded so that it is not everywhere zero. */
    uint64_t s[ 2 ];

    uint64_t xorshift128plus_int(void) {
        uint64_t s1 = s[0];
        const uint64_t s0 = s[1];
        const uint64_t result = s0 + s1;
        s[0] = s0;
        s1 ^= s1 << 23; // a
        s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
        return result;
    }

    double xorshift128plus_double(void) {
        const uint64_t x = xorshift128plus_int();
        const uint64_t x_doublefied = UINT64_C(0x3FF) << 52 | x >> 12;
        return *((double *) &x_doublefied) - 1.0;
    }

    float xorshift128plus_float(void) {
        const uint64_t x = xorshift128plus_int();
        const uint32_t x_floatified = 0x7Fu << 23 | (x & 0xffffffff) >> 9;
        return *((float *) & x_floatified) - 1.f;
    }

public:

    explicit Random(int64_t seed) 
    {
        set_seed(seed ? seed : 0x13371337cafebabe); // 0 is illegal so remap it to some value
    }

    void set_seed(int64_t seed) { s[0] = seed; s[1] = seed >> 32; }

    float nextf()
    {
        return xorshift128plus_float();
    }

    float nextf(float min_, float max_)
    {
        return nextf() * (max_ - min_) + min_;
    }

    double nextd()
    {
        return xorshift128plus_double();
    }

    double nextd(double min_, double max_)
    {
        return nextd() * (max_ - min_) + min_;
    }

};


}}

#endif
