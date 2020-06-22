#include <tuple>

#include "shaders/flat_shader.h"

FlatShader::FlatShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf) : 
    Shader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf) {}

Vec4f FlatShader::Vertex(const FaceInfo &face, int vertex_index) {
    // Avoid recalculating the intensity for all triangle's vertices
    if (vertex_index == 0) {
        Vec3f normal = (face.v[1] - face.v[0]).To<Vec3f, 3>()
            .Cross((face.v[2] - face.v[0]).To<Vec3f, 3>())
            .Normalize();
        normal = (mvp_inverse_transpose * normal.To<Vec4f, 4>())
            .ProjectTo3d()
            .To<Vec3f, 3>()
            .Normalize();

        intensity = normal.Dot(-light_dir_in_camera_space);
    }


    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> FlatShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    Vec2f uv = Uv(barycentric, face);
    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * (intensity - HardShadowIntensity(barycentric));
    return std::make_tuple(true, Color(tex_color));
}