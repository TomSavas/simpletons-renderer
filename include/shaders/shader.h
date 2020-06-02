#ifndef SHADER_H
#define SHADER_H

#include <tuple>

#include "matrix.hpp"
#include "vec.hpp"

#include "color.h"
#include "model.h"

class Shader {
public:
    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp);
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, const FaceInfo &face, TGAImage &tex);
};

#endif