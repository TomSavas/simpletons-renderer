#include <tuple>

#include "shaders/phong_shader.h"

PhongShader::PhongShader(TGAImage *tex, Vec3f light_dir) : Shader(tex), light_dir(light_dir) {}

Vec4f PhongShader::Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) {
    normals[vertex_index] = face.n[vertex_index].Normalize();

    return Shader::Vertex(face, vertex_index, mvp);
}

std::tuple<bool, Color> PhongShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    Vec3f normal(barycentric.X() * normals[0].X() + barycentric.Y() * normals[1].X() + barycentric.Z() * normals[2].X(),
                 barycentric.X() * normals[0].Y() + barycentric.Y() * normals[1].Y() + barycentric.Z() * normals[2].Y(),
                 barycentric.X() * normals[0].Z() + barycentric.Y() * normals[1].Z() + barycentric.Z() * normals[2].Z());

    float intensity = normal.Normalize().Dot(light_dir);
    TGAColor tex_color = tex->get(uv.X(), uv.Y()) * intensity;

    return std::make_tuple(true, Color(tex_color));
}