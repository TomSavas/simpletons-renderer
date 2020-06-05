#ifndef SHADER_H
#define SHADER_H

#include <tuple>

#include "matrix.hpp"
#include "vec.hpp"

#include "color.h"
#include "model.h"

class Shader {
protected:
    TGAImage &tex;
    Mat4f mvp;
    Vec3f original_light_dir;
    Vec3f light_dir;

    Vec2f Uv(const Vec3f &barycentric, const FaceInfo &face) const;
    Vec2f UvTexScaled(const Vec3f &barycentric, const FaceInfo &face) const;

    virtual void Mvp(const Mat4f &mvp);
    virtual void LightDir(const Vec3f &light_dir);
    virtual void CalcLightDir();

public:
    Shader(TGAImage &tex, const Mat4f &mvp, const Vec3f &light_dir);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index);
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, const FaceInfo &face);
};

#endif