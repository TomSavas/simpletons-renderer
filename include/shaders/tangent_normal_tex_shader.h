#ifndef TANGENT_NORMAL_TEX_SHADER_H
#define TANGENT_NORMAL_TEX_SHADER_H

#include "shaders/normal_tex_shader.h"
#include "tgaimage.h"
#include "vec.hpp"

class TangentNormalTexShader : public NormalTexShader {
private:
    Mat3f basis_equation_coeffs;

public:
    TangentNormalTexShader(TGAImage &tex, const Mat4f &mvp,
        const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
        const IFramebuf &framebuf, const std::vector<float> &shadow_buf, TGAImage &normal_tex);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index) override;
    virtual std::tuple<bool, Color> Fragment(const Bary3f &barycentric,
        const FaceInfo &face) override;
};

#endif