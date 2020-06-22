#include <tuple>

#include "shaders/phong_shader.h"

PhongShader::PhongShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf) :
    Shader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf) {}

Vec4f PhongShader::Vertex(const FaceInfo &face, int vertex_index) {
    normals[vertex_index] = (mvp_inverse_transpose * face.n[vertex_index].To<Vec4f, 4>())
        .ProjectTo3d()
        .To<Vec3f, 3>()
        .Normalize();

    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> PhongShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    Vec3f normal = barycentric.ApplyOn(normals);

    float intensity = normal.Dot(-light_dir_in_camera_space) - HardShadowIntensity(barycentric);

    Vec2f uv = Uv(barycentric, face);
    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;
    return std::make_tuple(true, Color(tex_color));
}