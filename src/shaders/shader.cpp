#include <tuple>

#include "matrix.hpp"
#include "shaders/shader.h"

Vec2f Shader::Uv(const Vec3f &barycentric, const FaceInfo &face) const {
    return Vec2f(barycentric.X() * face.uv[0].X() + barycentric.Y() * face.uv[1].X() + barycentric.Z() * face.uv[2].X(),
        barycentric.X() * face.uv[0].Y() + barycentric.Y() * face.uv[1].Y() + barycentric.Z() * face.uv[2].Y());
}

Vec2f Shader::UvTexScaled(const Vec3f &barycentric, const FaceInfo &face) const {
    Vec2f uv = Uv(barycentric, face);
    uv.X() *= tex.get_width();
    uv.Y() *= tex.get_height();

    return uv;
}

void Shader::CalcLightDir() {
    light_dir = (mvp * original_light_dir.To<Vec4f, 4>()).ProjectTo3d().To<Vec3f, 3>().Normalize();
}

Shader::Shader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir) : tex(tex), mvp(mvp),
    original_light_dir(light_dir) {
    CalcLightDir();
}

// Just apply MVP by default
Vec4f Shader::Vertex(const FaceInfo &face, int vertex_index) {
    return mvp * face.v[vertex_index];
}

// Simply apply the given texture and leave out the lighting
std::tuple<bool, Color> Shader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    TGAColor tex_color = tex.get(uv.X(), uv.Y());
    printf("r: %d, g: %d, b: %d, a:%d\n", tex_color[0], tex_color[1], tex_color[2], tex_color[3]);
    return std::make_tuple(true, Color(tex_color));
}