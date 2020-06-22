#ifndef RENDERER_H
#define RENDERER_H

#include "matrix.hpp"
#include "vec.hpp"

#include "color.h"
#include "framebuf.h"
#include "model.h"
#include "shaders/shader.h"
#include "tgaimage.h"

class Renderer {
private:
    Framebuf fb;
    std::vector<float> *z_buf;
    std::vector<float> *shadow_buf;

    const Mat4f &projection;
    const Mat4f &view;
    const Vec3f &camera_pos;
    const Vec3f &light_dir;

    const Mat4f &light_view;

    Mat4f current_mvp;
    Mat4f current_light_mvp;

    int BufIndex(int x, int y) const;

    void DrawTriangle(const Model &model, Shader &shader, const FaceInfo &face, TGAImage &tex,
        std::vector<float> &depth_buf, IFramebuf &framebuf);
    void DrawTriangle(const Model &model, Shader &shader, const FaceInfo &face, TGAImage &tex,
        std::vector<float> &depth_buf);
    void DrawLine(int x0, int y0, int x1, int y1, Color color);

public:
    Renderer(const Mat4f &projection_mat, const Mat4f &view_mat, const Vec3f &camera_pos,
        const Vec3f &light_dir, const Mat4f &light_view_mat);
    ~Renderer();

    void DrawLine(Vec4f line_start, Vec4f line_end, Color color);
    void DrawModel(const Model &model, TGAImage &tex, TGAImage &normal_tex, const Mat4f &model_mat);
    void DrawAxes();
};

#endif
