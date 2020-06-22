#include <tuple>

#include "shaders/gouraud_shader.h"

GouraudShader::GouraudShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf) : 
    Shader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf) {}

Vec4f GouraudShader::Vertex(const FaceInfo &face, int vertex_index) {
    // Save lighting intensities as a vector so that we can take a dot product of it
    // and barycentric coordinates later on easily
    intensities[vertex_index] = (mvp_inverse_transpose * face.n[vertex_index].To<Vec4f, 4>())
        .ProjectTo3d()
        .To<Vec3f, 3>()
        .Normalize()
        .Dot(-light_dir_in_camera_space);

    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> GouraudShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    float intensity = intensities.Dot(barycentric) - HardShadowIntensity(barycentric);

    Vec2f uv = Uv(barycentric, face);
    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;
    return std::make_tuple(true, Color(tex_color));
}