#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "vec.hpp"

typedef struct {
    int vertex_index;
    int uv_index;
    int normal_index;
} FaceInfoIndices;

typedef struct {
    Vec4f v0, v1, v2;
    Vec3f uv0, uv1, uv2;
    Vec3f n0, n1, n2;
} FaceInfo;

class Model {
private:
    std::vector<Vec4f> *vertices;
    std::vector<Vec3f> *uvs;
    std::vector<Vec3f> *normals;
    std::vector<std::vector<FaceInfoIndices>*> *faces;

public:
    Model(const char *filepath);
    ~Model();

    int VertCount() const;
    int FaceCount() const;

    Vec4f Vert(int index) const;
    Vec3f Uv(int index) const;
    Vec3f Normal(int index) const;
    const std::vector<FaceInfoIndices> &FaceIndices(int index) const;
    FaceInfo Face(int index) const;
};

#endif
