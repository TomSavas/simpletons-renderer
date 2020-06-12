#ifndef NORMAL_TEX_SHADER_H
#define NORMAL_TEX_SHADER_H

#include "shaders/shader.h"
#include "tgaimage.h"
#include "vec.hpp"

class NormalTexShader : public Shader {
protected:
    TGAImage &normal_tex;
    Mat4f mvp_inverse_transpose;

public:
    NormalTexShader(TGAImage &tex, TGAImage &normal_tex, const Mat4f &mvp, const Vec3f &light_dir);

    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric,
        const FaceInfo &face) override;
};

#endif