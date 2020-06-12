#ifndef TANGENT_NORMAL_TEX_SHADER_H
#define TANGENT_NORMAL_TEX_SHADER_H

#include "shaders/normal_tex_shader.h"
#include "tgaimage.h"
#include "vec.hpp"

class TangentNormalTexShader : public NormalTexShader {
private:
    Mat3f basis_equation_coeffs;

public:
    TangentNormalTexShader(TGAImage &tex, TGAImage &normal_tex, const Mat4f &mvp, const Vec3f &light_dir);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index) override;
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric,
        const FaceInfo &face) override;
};

#endif