
#include "LabVfx/Render.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"
#include "dbgui/dbgui.h"
#include "instancing_glsl.h"

#define MSAA_SAMPLES (4)

namespace lab { namespace vfx {

struct RenderGraphicsData
{
    RenderGraphicsData();
    sg_pipeline pip;
    sg_pass_action pass_action;
    sg_bindings bind;
};

RenderGraphicsData::RenderGraphicsData() 
{
    memset(&pip, 0, sizeof(pip));
    memset(&pass_action, 0, sizeof(pass_action));
    memset(&bind, 0, sizeof(bind));
    
    __dbgui_setup(MSAA_SAMPLES);

    pass_action.colors[0] = { SG_ACTION_CLEAR, { 0.0f, 0.0f, 0.0f, 1.0f } };

    /* vertex buffer for static geometry, goes into vertex-buffer-slot 0 */
    const float r = 0.05f;
    const float vertices[] = {
        // positions            colors
        0.0f,   -r, 0.0f,       1.0f, 0.0f, 0.0f, 1.0f,
           r, 0.0f, r,          0.0f, 1.0f, 0.0f, 1.0f,
           r, 0.0f, -r,         0.0f, 0.0f, 1.0f, 1.0f,
          -r, 0.0f, -r,         1.0f, 1.0f, 0.0f, 1.0f,
          -r, 0.0f, r,          0.0f, 1.0f, 1.0f, 1.0f,
        0.0f,    r, 0.0f,       1.0f, 0.0f, 1.0f, 1.0f
    };

    sg_buffer_desc buffer_desc;
    memset(&buffer_desc, 0, sizeof(sg_buffer_desc));
    buffer_desc.type = _SG_BUFFERTYPE_DEFAULT;
    buffer_desc.size = sizeof(vertices);
    buffer_desc.content = vertices;
    buffer_desc.label = "geometry-vertices";
    bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);

    /* index buffer for static geometry */
    const uint16_t indices[] = {
        0, 1, 2,    0, 2, 3,    0, 3, 4,    0, 4, 1,
        5, 1, 2,    5, 2, 3,    5, 3, 4,    5, 4, 1
    };
    memset(&buffer_desc, 0, sizeof(sg_buffer_desc));
    buffer_desc.type = SG_BUFFERTYPE_INDEXBUFFER;
    buffer_desc.size = sizeof(indices);
    buffer_desc.content = indices;
    buffer_desc.label = "geometry-indices";
    bind.index_buffer = sg_make_buffer(&buffer_desc);

    /* empty, dynamic instance-data vertex buffer, goes into vertex-buffer-slot 1 */
    memset(&buffer_desc, 0, sizeof(sg_buffer_desc));
    buffer_desc.type = _SG_BUFFERTYPE_DEFAULT;
    buffer_desc.size = MAX_PARTICLES * sizeof(hmm_vec3);
    buffer_desc.usage = SG_USAGE_STREAM;
    buffer_desc.label = "instance-data";
    bind.vertex_buffers[1] = sg_make_buffer(&buffer_desc);

    /* a shader */
    sg_shader shd = sg_make_shader(instancing_shader_desc());

    /* a pipeline object */
    sg_pipeline_desc pipeline_desc;
    memset(&pipeline_desc, 0, sizeof(sg_pipeline_desc));
    /* vertex buffer at slot 1 must step per instance */
    pipeline_desc.layout.buffers[1].step_func = SG_VERTEXSTEP_PER_INSTANCE;
    pipeline_desc.layout.attrs[ATTR_vs_pos] = { 0,0, SG_VERTEXFORMAT_FLOAT3 }; // buffer_index, offset, format
    pipeline_desc.layout.attrs[ATTR_vs_color0] = { 0,0, SG_VERTEXFORMAT_FLOAT4 };
    pipeline_desc.layout.attrs[ATTR_vs_inst_pos] = { 1,0, SG_VERTEXFORMAT_FLOAT3 };
    pipeline_desc.shader = shd;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
    pipeline_desc.depth_stencil.depth_write_enabled = true;
    pipeline_desc.rasterizer.cull_mode = SG_CULLMODE_BACK;
    pipeline_desc.rasterizer.sample_count = MSAA_SAMPLES;
    pipeline_desc.label = "instancing-pipeline";
    pip = sg_make_pipeline(&pipeline_desc);
}

Render::Render(std::shared_ptr<Effect> p) : _effect(p) 
{
    _data = new RenderGraphicsData(); 
}
Render::~Render()
{
    delete _data;
}

void Render::render(const float* mvp)
{

}
 

}}

#if 0

#include "Particles.h"
#include "gl4.h"

#include <iostream>

struct Temp {
    Temp() : geomData(logThrowOnError), needInit(true), valid(false) { }
    
    Buffer<vec3> verts;
    VAO geomData;
    Shader pointShader;
    bool needInit;
    bool valid;
    
    void init() {
        try {
            valid = false;
            needInit = false;
            pointShader.vertexShader(glsl(
                uniform mat4 mvp;
                in vec3 vertex;
                void main() {
                    gl_Position = mvp * vec4(vertex, 1);
                }
            )).fragmentShader(glsl(
                out vec4 fragColor;
                void main() {
                    fragColor = vec4(1,1,1,1);
                }
            )).link();
            verts.upload();
            geomData.create(pointShader, verts).attribute<float>("vertex", 3).check();
            valid = true;
        }
        catch(std::exception& exc) {
            std::cout << exc.what() << std::endl;
        }
    }
};

Temp temp;

void Render::render(const float* mvp) {
    if (!_particles)
        return;
    
    return;
    
    const DataStripes& stripes = _particles->dataStripes();
    int count = stripes.activeCount();
    if (!count)
        return;
    
    if (temp.needInit)
        temp.init();
    
    if (!temp.valid)
        return;

    const std::vector<int>& redirect = _particles->dataStripes().redirect();

    std::shared_ptr<DataStripe> pos = _particles->dataStripes().get("pos");
    float* data = pos->data<float>(DataStripe::kFloat32_3);
    int stride = pos->stride();
    if (!data)
        return;
    
    // todo pack data interleaved
    // cf. http://developer.apple.com/library/ios/#DOCUMENTATION/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

    temp.verts.data.clear();
    temp.verts.data.reserve(count);
    for (int i = 0; i < count; ++i) {
        int index = redirect[i];
        float* p = &data[index * stride];
        temp.verts << vec3(p[0], p[1], p[2]);
    }
    
    // todo stream buffer should be double buffered
    // cf. http://developer.apple.com/library/ios/#DOCUMENTATION/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/OpenGLESApplicationDesign/OpenGLESApplicationDesign.html#//apple_ref/doc/uid/TP40008793-CH6-SW5

    temp.verts.upload(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    if (temp.needInit)
        temp.init();
    
    temp.pointShader.use();
    temp.pointShader.uniform("mvp", *(mat4*)mvp);
    temp.geomData.draw(GL_POINTS);
    temp.pointShader.unuse();
}

#endif
