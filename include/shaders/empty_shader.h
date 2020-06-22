#ifndef _EMPTY_SHADER_H
#define _EMPTY_SHADER_H

#include <vector>

#include "shaders/shader.h"

class EmptyShader : public Shader {
public:
    EmptyShader(TGAImage &tex, const Mat4f &mvp,
        const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
        const IFramebuf &framebuf, const std::vector<float> &shadow_buf);

    virtual std::tuple<bool, Color> Fragment(const Bary3f &barycentric,
        const FaceInfo &face) override;
};

#endif