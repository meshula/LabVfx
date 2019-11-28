
#ifndef LABVFX_RENDER_H
#define LABVFX_RENDER_H

#include "LabVfx.h"
#include <memory>

namespace lab { namespace vfx {

struct RenderGraphicsData;

class Render {
public:
    Render(std::shared_ptr<Effect> p);
    ~Render();

    void render(const float* mvp);
    
private:
    std::shared_ptr<Effect> _effect;
    RenderGraphicsData* _data;
};

}}

#endif
