#ifndef RENDERER_H
#define RENDERER_H

#include "matrix.hpp"
#include "vec.hpp"

#include "color.h"
#include "framebuf.h"
#include "model.h"
#include "shader.h"
#include "tgaimage.h"

class Renderer {
private:
    Framebuf fb;
    std::vector<float> *z_buf;
    Mat4f projection;
    Mat4f view;

    int ZBufIndex(int x, int y) const;

    void DrawLine(int x0, int y0, int x1, int y1, Color color);

public:
    Renderer(Mat4f projection_mat, Mat4f view_mat);

    void DrawLine(Vec4f line_start, Vec4f line_end, Color color);
    void DrawTriangle(const Model &model, Shader &shader, const Mat4f &mvp,
        const std::vector<FaceInfoIndices> &face_indices, const FaceInfo &face, TGAImage &tex);
    void DrawModel(const Model &model, TGAImage &tex, Mat4f model_mat);

    void DrawAxes();

    void SetProjection(Mat4f projection_mat);
    void SetView(Mat4f view_mat);
};

#endif
