#include <fstream>
#include <iostream>
#include <sstream>

#include "model.h"

// Stolen directly from https://github.com/ssloy/tinyrenderer
// Will redo in the nearest future
Model::Model(const char *filepath) {
    vertices = new std::vector<Vec3f>();
    faces = new std::vector<std::vector<int>*>();
    float max_value = 1;

    std::ifstream in;
    in.open (filepath, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f vertex;
            iss >> vertex.x;
            iss >> vertex.y;
            iss >> vertex.z;

            max_value = std::max(max_value, std::max(vertex.x, std::max(vertex.y, vertex.z)));

            vertices->push_back(vertex);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> *f = new std::vector<int>();
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f->push_back(idx);
            }
            faces->push_back(f);
        }
    }

    for (int i = 0; i < vertices->size(); i++) {
        (*vertices)[i] = Vec3f((*vertices)[i].x / max_value + 1, 
                (*vertices)[i].y / max_value + 1,
                (*vertices)[i].z / max_value + 1);

    }

    std::cerr << "# v# " << vertices->size() << " f# "  << faces->size() << std::endl;
}

Model::~Model() {
    free(vertices);

    for (int i = 0; i < (*faces)[i]->size(); i++) {
        free((*faces)[i]);
    }

    free(faces);
}

int Model::VertCount() const {
    return vertices->size();
}

int Model::FaceCount() const {
    return faces->size();
}

Vec3f Model::Vert(int index) const {
    return vertices->operator[](index);
}

std::vector<int> *Model::Face(int index) const {
    return faces->operator[](index);
}
