#ifndef RENDERER_H
#define RENDERER_H

#include "framebuf.h"
#include "vec.h"
#include "color.h"
#include "model.h"
#include "tgaimage.h"

class Renderer {
private:
    Framebuf fb;
    std::vector<float> *z_buf;

    int ZBufIndex(int x, int y) const;

    void DrawLine(int x0, int y0, int x1, int y1, Color color);

public:
    Renderer();

    void DrawLine(Vec3f line_start, Vec3f line_end, Color color);
    void DrawTriangle(Vec3f t0, Vec3f t1, Vec3f t2, Vec3f uv0, Vec3f uv1, Vec3f uv2, Color color);
    void DrawTriangle(Vec3f t0, Vec3f t1, Vec3f t2, Vec3f uv0, Vec3f uv1, Vec3f uv2,
            float lighting_intensity, TGAImage &tex);
    void DrawModel(const Model &model, TGAImage &tex, Vec3f insertion_point, float scale);
};

#endif
