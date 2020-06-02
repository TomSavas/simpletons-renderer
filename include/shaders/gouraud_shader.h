#ifndef GOURAUD_SHADER_H
#define GOURAUD_SHADER_H

#include "shaders/shader.h"

class GouraudShader : public Shader {
protected:
    Vec3f intensities;
    Vec3f light_dir;
public:
    GouraudShader(Vec3f light_dir) : light_dir(light_dir) {}

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) override;
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, const FaceInfo &face,
        TGAImage &tex) override;
};

#endif