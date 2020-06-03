#include <algorithm>
#include <limits>
#include <stdlib.h>
#include <tuple>

#include "matrix.hpp"

#include "renderer.h"
#include "shaders/gouraud_shader.h"
#include "shaders/shader.h"
#include "shaders/toon_shader.h"

Renderer::Renderer(Mat4f projection_mat, Mat4f view_mat) : fb(), projection(projection_mat), view(view_mat) {
    z_buf = new std::vector<float>(fb.Width() * fb.Height(), -std::numeric_limits<float>::max());
}

int Renderer::ZBufIndex(int x, int y) const {
    return x + y * fb.Height();
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, Color color) {
    int x_diff = x1 - x0;
    int y_diff = y1 - y0;
    bool xy_swapped = false;

    if (std::abs(y_diff) > std::abs(x_diff)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(x_diff, y_diff);

        xy_swapped = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++) {
        float line_percentage = (float)(x - x0) / x_diff;
        int y = ((float)y_diff * line_percentage) + y0;

        if (xy_swapped)
            fb.Set(y, x, color);
        else
            fb.Set(x, y, color);
    }
}

void Renderer::DrawLine(Vec4f v0, Vec4f v1, Color color, const Mat4f &mvp) { 
    v0 = fb.ViewportMatrix() * (mvp * v0).ProjectTo3d();
    v1 = fb.ViewportMatrix() * (mvp * v1).ProjectTo3d();

    DrawLine(v0.X(), v0.Y(), v1.X(), v1.Y(), color);
}

void Renderer::DrawTriangle(const Model &model, Shader &shader, const Mat4f &mvp,
    const FaceInfo &face, TGAImage &tex) {
    Vec4f v0 = fb.ViewportMatrix() * shader.Vertex(face, 0, mvp).ProjectTo3d();
    Vec4f v1 = fb.ViewportMatrix() * shader.Vertex(face, 1, mvp).ProjectTo3d();
    Vec4f v2 = fb.ViewportMatrix() * shader.Vertex(face, 2, mvp).ProjectTo3d();

    float min_x = std::min(v0.X(), std::min(v1.X(), v2.X()));
    float min_y = std::min(v0.Y(), std::min(v1.Y(), v2.Y()));

    float max_x = std::max(v0.X(), std::max(v1.X(), v2.X()));
    float max_y = std::max(v0.Y(), std::max(v1.Y(), v2.Y()));

    // Could do binary search on x to see where the triangle starts if the image is bigger than say 64?
    for (int y = min_y; y <= max_y; y++) {
        // Ignore the point if it's out of viewport's bounds
        if (y < 0 || y >= fb.Height())
            continue;

        bool drawing_triangle = false;

        for (int x = min_x; x <= max_x; x++) {
            // Ignore the point if it's out of viewport's bounds
            if (x < 0 || x >= fb.Width())
                continue;

            Vec3f barycentric = Vec3f(x, y).Barycentric(v0.To<Vec3f, 3>(), v1.To<Vec3f, 3>(), v2.To<Vec3f, 3>());
            if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0) {
                // If we are already drawing a triangle and we stepped out of it
                // we won't be drawing it again on this line.
                if (drawing_triangle)
                    break;

                continue;
            }
            drawing_triangle = true;

            bool should_render;
            Color color;
            std::tie(should_render, color) = shader.Fragment(barycentric, face);

            if (!should_render)
                continue;

            float z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();
            int z_buf_index = ZBufIndex(x, y);
            if (z > z_buf->operator[](z_buf_index)) {
                z_buf->operator[](z_buf_index) = z;
                fb.Set(x, y, color);
            }
        }
    }
}

void Renderer::DrawModel(const Model &model, TGAImage &tex, TGAImage &normal_tex, Mat4f model_mat) {
    Mat4f mvp = projection * view * model_mat;
    Shader *shader = new GouraudShader(&tex, Vec3f(0, 0, 1));

    for (int i = 0; i < model.FaceCount(); i++)
        DrawTriangle(model, *shader, mvp, model.Face(i), tex);

    delete shader;
}

void Renderer::DrawAxes() {
    Mat4f mvp = projection * view;
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0.25, 0, 0, 1), Color(255, 0, 0, 0), mvp);
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0, 0.25, 0, 1), Color(0, 255, 0, 0), mvp);
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0, 0, 0.25, 1), Color(0, 0, 255, 0), mvp);
}
