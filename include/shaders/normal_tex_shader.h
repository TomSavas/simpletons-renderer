#ifndef NORMAL_TEX_SHADER_H
#define NORMAL_TEX_SHADER_H

#include "shaders/shader.h"
#include "tgaimage.h"
#include "vec.hpp"

class NormalTexShader : public Shader {
protected:
    TGAImage &normal_tex;

public:
    NormalTexShader(TGAImage &tex, const Mat4f &mvp,
        const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
        const IFramebuf &framebuf, const std::vector<float> &shadow_buf, TGAImage &normal_tex);

    virtual std::tuple<bool, Color> Fragment(const Bary3f &barycentric,
        const FaceInfo &face) override;
};

#endif