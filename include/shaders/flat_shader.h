#ifndef FLAT_SHADER_H
#define FLAT_SHADER_H

#include "shaders/shader.h"

class FlatShader : public Shader {
protected:
    float intensity;

public:
    FlatShader(TGAImage &tex, const Mat4f &mvp, const Mat4f &camera_viewport_to_shadow_viewport,
        const Vec3f &light_dir, const IFramebuf &framebuf, const std::vector<float> &shadow_buf);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index) override;
    virtual std::tuple<bool, Color> Fragment(const Bary3f &barycentric, 
        const FaceInfo &face) override;
};

#endif