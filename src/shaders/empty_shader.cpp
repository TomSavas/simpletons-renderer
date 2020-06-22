#include "shaders/empty_shader.h"

EmptyShader::EmptyShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf) :
    Shader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf) {}

std::tuple<bool, Color> EmptyShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    return std::make_tuple(true, Color(0, 0, 0, 0));
}