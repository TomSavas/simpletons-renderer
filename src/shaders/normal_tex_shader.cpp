#include "shaders/normal_tex_shader.h"

NormalTexShader::NormalTexShader(TGAImage &tex, TGAImage &normal_tex, const Mat4f &mvp,
    const Vec3f &light_dir) : Shader(tex, mvp, light_dir), normal_tex(normal_tex) {}

std::tuple<bool, Color> NormalTexShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    TGAColor normal_as_color = normal_tex.get(uv.X(), uv.Y());

    Vec3f normal((float)normal_as_color[2] - 255.0/2.0 , (float)normal_as_color[1] - 255.0/2.0, (float)normal_as_color[0] - 255.0/2.0);
    normal = normal.Normalize();
    normal = (mvp.Inverse().Transpose() * normal.To<Vec4f, 4>()).To<Vec3f, 3>().Normalize();
    float intensity = normal.Dot(light_dir);

    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;
    return std::make_tuple(true, Color(tex_color));
}