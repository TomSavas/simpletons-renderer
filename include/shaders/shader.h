#ifndef SHADER_H
#define SHADER_H

#include <tuple>
#include <vector>

#include "matrix.hpp"
#include "vec.hpp"

#include "color.h"
#include "iframebuf.h"
#include "tgaimage.h"
#include "model.h"

class Shader {
protected:
    TGAImage &tex;
    const Mat4f &mvp;
    const Mat4f &camera_viewport_to_shadow_viewport;
    const Vec3f &original_light_dir;
    const IFramebuf &framebuf;
    const std::vector<float> &shadow_buf;

    Mat4f mvp_inverse_transpose;
    Vec3f light_dir_in_camera_space;
    Vec4f ndc_verts[3];

    Vec2f Uv(const Bary3f &barycentric, const FaceInfo &face) const;
    float HardShadowIntensity(const Bary3f &barycentric) const;

public:
    Shader(TGAImage &tex, const Mat4f &mvp, const Mat4f &camera_viewport_to_shadow_viewport,
        const Vec3f &light_dir, const IFramebuf &framebuf, const std::vector<float> &shadow_buf);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index);
    virtual std::tuple<bool, Color> Fragment(const Bary3f &barycentric, const FaceInfo &face);
};

#endif