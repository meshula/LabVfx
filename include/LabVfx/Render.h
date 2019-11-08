
#pragma once

#include "Forward.h"
#include <memory>

class Render {
public:
    Render(std::shared_ptr<Particles> p) : _particles(p) { }

    void render(const float* mvp);
    
private:
    std::shared_ptr<Particles> _particles;
};
