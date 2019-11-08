
#include "Render.h"
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
