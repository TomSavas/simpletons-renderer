#include "shaders/tangent_normal_tex_shader.h"

TangentNormalTexShader::TangentNormalTexShader(TGAImage &tex, TGAImage &normal_tex, const Mat4f &mvp,
    const Vec3f &light_dir) : NormalTexShader(tex, normal_tex, mvp, light_dir) {}

Vec4f TangentNormalTexShader::Vertex(const FaceInfo &face, int vertex_index) {
    if (vertex_index != 0)
        return NormalTexShader::Vertex(face, vertex_index);

    Vec4f a = face.v[1] - face.v[0];
    Vec4f b = face.v[2] - face.v[0];

    basis_equation_coeffs[0][0] = a.X();
    basis_equation_coeffs[0][1] = a.Y();
    basis_equation_coeffs[0][2] = a.Z();

    basis_equation_coeffs[1][0] = b.X();
    basis_equation_coeffs[1][1] = b.Y();
    basis_equation_coeffs[1][2] = b.Z();

    basis_equation_coeffs[2][0] = 0;
    basis_equation_coeffs[2][1] = 0;
    basis_equation_coeffs[2][2] = 0;

    return Shader::Vertex(face, vertex_index);
}

std::tuple<bool, Color> TangentNormalTexShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

    Vec2f uv = UvTexScaled(barycentric, face);

    Vec3f normal(barycentric.X() * face.n[0].X() + barycentric.Y() * face.n[1].X() + barycentric.Z() * face.n[2].X(),
                 barycentric.X() * face.n[0].Y() + barycentric.Y() * face.n[1].Y() + barycentric.Z() * face.n[2].Y(),
                 barycentric.X() * face.n[0].Z() + barycentric.Y() * face.n[1].Z() + barycentric.Z() * face.n[2].Z());

    normal = (mvp_inverse_transpose * normal.To<Vec4f, 4>()).To<Vec3f, 3>().Normalize();

    basis_equation_coeffs[2][0] = normal.X();
    basis_equation_coeffs[2][1] = normal.Y();
    basis_equation_coeffs[2][2] = normal.Z();

    Mat3f inverse = basis_equation_coeffs.Inverse();

    // Our tangent space basis vectors are i, j, n as x, y, z respectively
    Vec3f i = inverse * Vec3f(face.uv[1].X() - face.uv[0].X(), face.uv[2].X() - face.uv[0].X());
    Vec3f j = inverse * Vec3f(face.uv[1].Y() - face.uv[0].Y(), face.uv[2].Y() - face.uv[0].Y());

    Mat3f tangent_space_transform;
    tangent_space_transform[0][0] = i.X();
    tangent_space_transform[1][0] = i.Y();
    tangent_space_transform[2][0] = i.Z();

    tangent_space_transform[0][1] = j.X();
    tangent_space_transform[1][1] = j.Y();
    tangent_space_transform[2][1] = j.Z();

    tangent_space_transform[0][2] = normal.X();
    tangent_space_transform[1][2] = normal.Y();
    tangent_space_transform[2][2] = normal.Z();

    TGAColor normal_as_color = normal_tex.get(uv.X(), uv.Y());

    Vec3f normal_from_tex((float)normal_as_color[2] - 255.0/2.0 - 1.0,
        (float)normal_as_color[1] - 255.0/2.0 - 1.0, (float)normal_as_color[0] - 255.0/2.0 - 1.0);
    normal_from_tex = normal_from_tex.Normalize();
    normal = (tangent_space_transform * normal_from_tex).Normalize();

    float intensity = normal.Dot(light_dir);

    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;
    return std::make_tuple(true, Color(tex_color));
}