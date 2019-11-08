
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabParticle/DataStripes.h"

#include <string>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace lab { namespace particle {

DataStripe::DataStripe(Kind k, uint32_t c, const char* name) : _kind(k), _data(0), _name(name) {
    switch (_kind) {
        case kUInt32_1:  _elKind = kUInt32_1;  break;
        case kFloat32_1: _elKind = kFloat32_1; break;
        case kFloat32_2: _elKind = kFloat32_1; break;
        case kFloat32_3: _elKind = kFloat32_1; break;
        case kFloat32_4: _elKind = kFloat32_1; break;
        default:         _data = 0;            break;
    }
    resize(c);
}
DataStripe::~DataStripe() {
    free(_data);
}

void DataStripe::resize(uint32_t c) {
    if (_size != c) {
        _size =c ;
        free(_data);
        switch (_kind) {
            case kUInt32_1:  _data = malloc(sizeof(uint32_t) * 1 * c); break;
            case kFloat32_1: _data = malloc(sizeof(float)    * 1 * c); break;
            case kFloat32_2: _data = malloc(sizeof(float)    * 2 * c); break;
            case kFloat32_3: _data = malloc(sizeof(float)    * 2 * c); break;
            case kFloat32_4: _data = malloc(sizeof(float)    * 2 * c); break;
            default:         _data = 0;                                break;
        }
    }
}

}}
