#include <tuple>

#include "shaders/flat_shader.h"

FlatShader::FlatShader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir) : 
    Shader(tex, mvp, light_dir) {}

Vec4f FlatShader::Vertex(const FaceInfo &face, int vertex_index) {
    // Avoid recalculating the intensity for all triangle's vertices
    if (vertex_index == 0) {
        Vec3f normal = (face.v[1] - face.v[0]).To<Vec3f, 3>().Cross((face.v[2] - face.v[0]).To<Vec3f, 3>()).Normalize();
        normal = (mvp.Inverse().Transpose() * normal.To<Vec4f, 4>()).ProjectTo3d().To<Vec3f, 3>().Normalize();
        intensity = normal.Dot(light_dir);
    }

    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> FlatShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);
    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;

    return std::make_tuple(true, Color(tex_color));
}