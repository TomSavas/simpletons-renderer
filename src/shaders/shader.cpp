#include <tuple>

#include "shaders/shader.h"

// Just apply MVP by default
Vec4f Shader::Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) {
    return mvp * face.v[vertex_index];
}

// Simply apply the given texture by default
std::tuple<bool, Color> Shader::Fragment(const Vec3f &barycentric, const FaceInfo &face,
    TGAImage &tex) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    float u = barycentric.X() * face.uv[0].X() + barycentric.Y() * face.uv[1].X() + barycentric.Z() * face.uv[2].X();
    u = u * tex.get_width();
    float v = barycentric.X() * face.uv[0].Y() + barycentric.Y() * face.uv[1].Y() + barycentric.Z() * face.uv[2].Y();
    v = v * tex.get_height();

    TGAColor tex_color = tex.get(u, v);
    return std::make_tuple(true, Color(tex_color));
}