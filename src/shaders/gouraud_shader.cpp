#include <tuple>

#include "shaders/gouraud_shader.h"

GouraudShader::GouraudShader(TGAImage *tex, Vec3f light_dir) : Shader(tex), light_dir(light_dir) {}

Vec4f GouraudShader::Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) {
    // Save lighting intensities as a vector so that we can take a dot product of it
    // and barycentric coordinates later on easily
    intensities[vertex_index] = face.n[vertex_index].Normalize().Dot(light_dir);

    return Shader::Vertex(face, vertex_index, mvp);
}

std::tuple<bool, Color> GouraudShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    float intensity = intensities.Dot(barycentric);
    //printf("intensity: %f\n", intensity);
    TGAColor tex_color = tex->get(uv.X(), uv.Y()) * intensity;

    return std::make_tuple(true, Color(tex_color));
}