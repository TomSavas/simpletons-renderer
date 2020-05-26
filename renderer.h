#ifndef RENDERER_H
#define RENDERER_H

#include "framebuf.h"
#include "vec.h"
#include "color.h"
#include "model.h"

class Renderer {
private:
    Framebuf fb;

    void DrawLine(int x0, int y0, int x1, int y1, Color color);
    void DrawTriangleLineSweep(Vec3f t0, Vec3f t1, Vec3f t2, Color color);
    void DrawTriangleBarycentric(Vec3f t0, Vec3f t1, Vec3f t2, Color color);

public:
    Renderer();

    void DrawLine(Vec3f line_start, Vec3f line_end, Color color);
    void DrawTriangle(Vec3f t0, Vec3f t1, Vec3f t2, Color color);
    void DrawModel(const Model &model, Vec3f insertion_point, float scale);
};

#endif
