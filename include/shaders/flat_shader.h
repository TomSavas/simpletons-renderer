#ifndef FLAT_SHADER_H
#define FLAT_SHADER_H

#include "shaders/shader.h"

class FlatShader : public Shader {
protected:
    float intensity;
public:
    FlatShader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index) override;
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, 
        const FaceInfo &face) override;
};

#endif