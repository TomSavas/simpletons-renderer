#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "vec.h"

class Model {
private:
    std::vector<Vec3f> *vertices;
    std::vector<std::vector<int>*> *faces;

public:
    Model(const char *filepath);
    ~Model();

    int VertCount() const;
    int FaceCount() const;

    Vec3f Vert(int index) const;
    std::vector<int> *Face(int index) const;
};

#endif
