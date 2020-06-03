#include <tuple>

#include "shaders/shader.h"


Vec2f Shader::Uv(const Vec3f &barycentric, const FaceInfo &face) const {
    return Vec2f(barycentric.X() * face.uv[0].X() + barycentric.Y() * face.uv[1].X() + barycentric.Z() * face.uv[2].X(),
        barycentric.X() * face.uv[0].Y() + barycentric.Y() * face.uv[1].Y() + barycentric.Z() * face.uv[2].Y());
}

Vec2f Shader::Uv(const Vec3f &barycentric, const FaceInfo &face, TGAImage &tex) const {
    Vec2f uv = Uv(barycentric, face);
    uv.X() *= tex.get_width();
    uv.Y() *= tex.get_height();

    return uv;
}

// Just apply MVP by default
Vec4f Shader::Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) {
    return mvp * face.v[vertex_index];
}

// Simply apply the given texture by default
std::tuple<bool, Color> Shader::Fragment(const Vec3f &barycentric, const FaceInfo &face,
    TGAImage &tex) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = Uv(barycentric, face, tex);

    TGAColor tex_color = tex.get(uv.X(), uv.Y());
    return std::make_tuple(true, Color(tex_color));
}