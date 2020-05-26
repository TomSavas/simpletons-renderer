#include <fstream>
#include <iostream>
#include <sstream>

#include "model.h"

// Stolen directly from https://github.com/ssloy/tinyrenderer
// Will redo in the nearest future
Model::Model(const char *filepath) {
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
            vertices.push_back(vertex);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
    //std::cerr << "# v# " << vertices.size() << " f# "  << faces.size() << std::endl;
}

int Model::VertCount() const {
    return vertices.size();
}

int Model::FaceCount() const {
    return faces.size();
}

Vec3f Model::Vert(int index) const {
    return vertices[index];
}

std::vector<int> Model::Face(int index) const {
    return faces[index];
}
