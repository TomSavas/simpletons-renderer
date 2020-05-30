#ifndef RENDERER_H
#define RENDERER_H

#include "framebuf.h"
#include "vec.h"
#include "color.h"
#include "model.h"
#include "tgaimage.h"
#include "matrix.h"

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
    void DrawTriangle(Vec4f t0, Vec4f t1, Vec4f t2, Vec3f uv0, Vec3f uv1, Vec3f uv2,
            float lighting_intensity, TGAImage &tex);
    void DrawModel(const Model &model, TGAImage &tex, Mat4f model_mat);

    void DrawAxes();

    void SetProjection(Mat4f projection_mat);
    void SetView(Mat4f view_mat);
};

#endif
