#include <tuple>

#include "shaders/shader.h"

Shader::Shader(TGAImage &tex, const Mat4f &mvp, const Mat4f &camera_viewport_to_shadow_viewport,
    const Vec3f &light_dir, const IFramebuf &framebuf, const std::vector<float> &shadow_buf) : 
    tex(tex), mvp(mvp), camera_viewport_to_shadow_viewport(camera_viewport_to_shadow_viewport),
    original_light_dir(light_dir), framebuf(framebuf), shadow_buf(shadow_buf) {
    mvp_inverse_transpose = mvp.Inverse().Transpose();
    light_dir_in_camera_space = (mvp * original_light_dir.To<Vec4f, 4>())
        .ProjectTo3d()
        .To<Vec3f, 3>()
        .Normalize();
}

Vec2f Shader::Uv(const Bary3f &barycentric, const FaceInfo &face) const {
    Vec2f uv = barycentric.ApplyOn(face.uv[0], face.uv[1], face.uv[2]).To<Vec2f, 2>();
    uv.X() *= tex.get_width();
    uv.Y() *= tex.get_height();

    return uv;
}

float Shader::HardShadowIntensity(const Bary3f &barycentric) const {
    Vec4f position = barycentric.ApplyOn(ndc_verts);
    Vec4f position_in_shadow_space = (camera_viewport_to_shadow_viewport * position).ProjectTo3d();
    int shadow_buf_index = framebuf.Width() * int(position_in_shadow_space.Y()) + int(position_in_shadow_space.X());

    if (shadow_buf_index < 0 || shadow_buf_index > framebuf.Size())
        return 0;

    return (position_in_shadow_space.Z() < shadow_buf[shadow_buf_index] - 0.01) ? 0.3 : 0;
}

// Just apply MVP by default
Vec4f Shader::Vertex(const FaceInfo &face, int vertex_index) {
    Vec4f clip_space_vert = mvp * face.v[vertex_index];
    ndc_verts[vertex_index] = (framebuf.ViewportMatrix() * clip_space_vert).ProjectTo3d();

    return clip_space_vert;
}

// Simply apply the given texture and leave out the lighting
std::tuple<bool, Color> Shader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    Vec2f uv = Uv(barycentric, face);

    TGAColor tex_color = tex.get(uv.X(), uv.Y());
    return std::make_tuple(true, Color(tex_color));
}