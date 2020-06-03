#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "shaders/shader.h"

class PhongShader : public Shader {
protected:
    Vec3f normals[3];
    Vec3f light_dir;
public:
    PhongShader(TGAImage *tex, Vec3f light_dir);

    virtual Vec4f Vertex(const FaceInfo &face, int vertex_index, const Mat4f &mvp) override;
    virtual std::tuple<bool, Color> Fragment(const Vec3f &barycentric, 
        const FaceInfo &face) override;
};

#endif