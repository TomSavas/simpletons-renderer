#ifndef TOON_SHADER_H
#define TOON_SHADER_H

#include "shaders/gouraud_shader.h"

class ToonShader : public GouraudShader {
protected:
    Color color;
    int shade_count;
    float intensity_per_shade;
public:
    ToonShader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir, Color color,
        int shade_count);

    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric,
        const FaceInfo &face) override;
};

#endif