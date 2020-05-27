#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "vec.h"

typedef struct {
    int vertex_index;
    int uv_index;
    int normal_index;
} FaceIndices;

class Model {
private:
    std::vector<Vec3f> *vertices;
    std::vector<Vec3f> *uvs;
    std::vector<std::vector<FaceIndices>*> *faces;

public:
    Model(const char *filepath);
    ~Model();

    int VertCount() const;
    int FaceCount() const;

    Vec3f Uv(int index) const;
    Vec3f Vert(int index) const;
    std::vector<FaceIndices> *Face(int index) const;
};

#endif
