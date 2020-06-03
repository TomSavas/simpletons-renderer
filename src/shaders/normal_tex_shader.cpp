#include "shaders/normal_tex_shader.h"

NormalTexShader::NormalTexShader(TGAImage *tex, TGAImage *normal_tex, Vec3f light_dir) :
    Shader(tex), normal_tex(normal_tex), light_dir(light_dir) {}


std::tuple<bool, Color> NormalTexShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    TGAColor normal_as_color = normal_tex->get(uv.X(), uv.Y());
    Vec3f normal = Vec3f(normal_as_color[0], normal_as_color[1], normal_as_color[2]).Normalize();

    float intensity = normal.Dot(light_dir);

    TGAColor tex_color = tex->get(uv.X(), uv.Y()) * intensity;

    return std::make_tuple(true, Color(tex_color));
}