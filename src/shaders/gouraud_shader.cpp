#include <tuple>

#include "shaders/gouraud_shader.h"

GouraudShader::GouraudShader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir) : 
    Shader(tex, mvp, light_dir) {}

Vec4f GouraudShader::Vertex(const FaceInfo &face, int vertex_index) {
    // Save lighting intensities as a vector so that we can take a dot product of it
    // and barycentric coordinates later on easily
    intensities[vertex_index] = (mvp.Inverse().Transpose() * face.n[vertex_index].To<Vec4f, 4>())
        .ProjectTo3d()
        .To<Vec3f, 3>()
        .Normalize()
        .Dot(light_dir);

    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> GouraudShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    float intensity = intensities.Dot(barycentric);
    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;

    return std::make_tuple(true, Color(tex_color));
}