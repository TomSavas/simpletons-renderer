#include <fstream>
#include <iostream>
#include <sstream>

#include "model.h"

// Stolen directly from https://github.com/ssloy/tinyrenderer
// Will redo in the nearest future
Model::Model(const char *filepath) {
    vertices = new std::vector<Vec4f>();
    uvs = new std::vector<Vec3f>();
    normals = new std::vector<Vec3f>();
    faces = new std::vector<std::vector<FaceInfoIndices>*>();
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
            Vec4f vertex;
            iss >> vertex.X();
            iss >> vertex.Y();
            iss >> vertex.Z();
            vertex.W() = 1.0;

            max_value = std::max(max_value, std::max(vertex.X(), std::max(vertex.Y(), vertex.Z())));

            vertices->push_back(vertex);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec3f uv;

            iss >> uv.X();
            iss >> uv.Y();
            iss >> trash;

            uvs->push_back(uv);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f normal;

            iss >> normal.X();
            iss >> normal.Y();
            iss >> normal.Z();

            normals->push_back(normal);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<FaceInfoIndices> *f = new std::vector<FaceInfoIndices>();
            FaceInfoIndices face_info_indices;
            iss >> trash;
            while (iss >> face_info_indices.vertex_index >> trash 
                    >> face_info_indices.uv_index >> trash >> face_info_indices.normal_index) {
                // in wavefront obj all indices start at 1, not zero
                face_info_indices.vertex_index--;
                face_info_indices.uv_index--;
                face_info_indices.normal_index--;

                f->push_back(face_info_indices);
            }
            faces->push_back(f);
        }
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

Vec4f Model::Vert(int index) const {
    return vertices->operator[](index);
}

Vec3f Model::Uv(int index) const {
    return uvs->operator[](index);
}

Vec3f Model::Normal(int index) const {
    return normals->operator[](index);
}

const std::vector<FaceInfoIndices> &Model::FaceIndices(int index) const {
    return *faces->operator[](index);
}

FaceInfo Model::Face(int index) const {
    std::vector<FaceInfoIndices> face_indices = FaceIndices(index);

    return FaceInfo {
        .v0 = Vert(face_indices[0].vertex_index),
        .v1 = Vert(face_indices[1].vertex_index),
        .v2 = Vert(face_indices[2].vertex_index),

        .uv0 = Uv(face_indices[0].uv_index),
        .uv1 = Uv(face_indices[1].uv_index),
        .uv2 = Uv(face_indices[2].uv_index),

        .n0 = Normal(face_indices[0].uv_index),
        .n1 = Normal(face_indices[1].uv_index),
        .n2 = Normal(face_indices[2].uv_index)
    };
}