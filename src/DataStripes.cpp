
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabVfx/DataStripes.h"

#include <string>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace lab { namespace vfx {

DataStripe::DataStripe(Kind k, size_t c, const char* name) : _kind(k), _data(0), _name(name) {
    switch (_kind) {
        case kUInt32_1:  _elKind = kUInt32_1;  break;
        case kFloat32_1: _elKind = kFloat32_1; break;
        case kFloat32_2: _elKind = kFloat32_1; break;
        case kFloat32_3: _elKind = kFloat32_1; break;
        case kFloat32_4: _elKind = kFloat32_1; break;
        default:         _data = 0;            break;
    }
    resizeElementCount(c);
}

DataStripe::~DataStripe() {
    free(_data);
}

void DataStripe::resizeElementCount(size_t c) {
    resizeDataBytes(c * stride());
}

void DataStripe::resizeDataBytes(size_t c) {
    if (_byteSize != c) {
        free(_data);
        _byteSize = c;
        _data = malloc(c + 10240000);
    }
}

}}
