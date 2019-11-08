
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#pragma once

#include "Particles.h"

class ParticleSystemTestVelocity {
public:
    ParticleSystemTestVelocity();
    
    
    void update(float t, float dt);
    void render(const float* mvp);
    
private:
    std::shared_ptr<Particles> particles;
};

