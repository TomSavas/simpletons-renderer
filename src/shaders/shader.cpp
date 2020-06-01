#include <tuple>

#include "shader.h"

// Just apply MVP by default
Vec4f Shader::Vertex(const Vec4f &vertex, const std::vector<FaceInfoIndices> &face_indices,
    int face_index, const Mat4f &mvp) {
    return mvp * vertex;
}

// Simply apply the given texture by default
std::tuple<bool, Color> Shader::Fragment(const Vec3f &barycentric, const FaceInfo &face,
    TGAImage &tex) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    float u = barycentric.X() * face.uv0.X() + barycentric.Y() * face.uv1.X() + barycentric.Z() * face.uv2.X();
    u = u * tex.get_width();
    float v = barycentric.X() * face.uv0.Y() + barycentric.Y() * face.uv1.Y() + barycentric.Z() * face.uv2.Y();
    v = v * tex.get_height();

    TGAColor tex_color = tex.get(u, v);
    return std::make_tuple(true, Color(tex_color));
}