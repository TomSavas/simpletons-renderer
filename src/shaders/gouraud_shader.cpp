#include <tuple>

#include "shaders/gouraud_shader.h"

Vec4f GouraudShader::Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) {
    // Save lighting intensities as a vector so that we can take a dot product of it
    // and barycentric coordinates later on easily
    intensities[vertex_index] = face.n[vertex_index].Normalize().Dot(light_dir);

    return Shader::Vertex(face, vertex_index, mvp);
}

std::tuple<bool, Color> GouraudShader::Fragment(const Vec3f &barycentric, const FaceInfo &face,
    TGAImage &tex) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    float u = barycentric.X() * face.uv[0].X() + barycentric.Y() * face.uv[1].X() + barycentric.Z() * face.uv[2].X();
    u = u * tex.get_width();
    float v = barycentric.X() * face.uv[0].Y() + barycentric.Y() * face.uv[1].Y() + barycentric.Z() * face.uv[2].Y();
    v = v * tex.get_height();

    float intensity = intensities.Dot(barycentric);
    TGAColor tex_color = tex.get(u, v) * intensity;

    return std::make_tuple(true, Color(tex_color));
}