#ifndef GOURAUD_SHADER_H
#define GOURAUD_SHADER_H

#include "shaders/shader.h"

class GouraudShader : public Shader {
protected:
    Vec3f intensities;
public:
    GouraudShader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index) override;
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, 
        const FaceInfo &face) override;
};

#endif