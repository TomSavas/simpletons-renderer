#include <stdlib.h>
#include <algorithm>
#include <limits>

#include "renderer.h"

Renderer::Renderer() : fb() {
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

void Renderer::DrawLine(Vec3f v0, Vec3f v1, Color color) { 
    DrawLine(v0.X(), v0.Y(), v1.X(), v1.Y(), color);
}

void Renderer::DrawTriangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f uv0, Vec3f uv1, Vec3f uv2, Color color) {
    float min_x = std::min(v0.X(), std::min(v1.X(), v2.X()));
    float min_y = std::min(v0.Y(), std::min(v1.Y(), v2.Y()));

    float max_x = std::max(v0.X(), std::max(v1.X(), v2.X()));
    float max_y = std::max(v0.Y(), std::max(v1.Y(), v2.Y()));

    // Could do binary search on x to see where the triangle starts if the image is bigger than say 64?
    for (int y = min_y; y <= max_y; y++) {
        bool drawing_triangle = false;

        for (int x = min_x; x <= max_x; x++) {
            Vec3f point(x, y);
            Vec3f barycentric = point.Barycentric(v0, v1, v2);

            if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0) {
                // If we are already drawing a triangle and we stepped out of it
                // we won't be drawing it again on this line.
                if (drawing_triangle)
                    break;

                continue;
            }
            drawing_triangle = true;

            float point_z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();
            int z_buf_index = ZBufIndex(x, y);
            if (point.Z() > z_buf->operator[](z_buf_index)) {
                z_buf->operator[](z_buf_index) = point_z;
                fb.Set(x, y, color);
            }
        }
    }
}

void Renderer::DrawTriangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f uv0, Vec3f uv1, Vec3f uv2,
        float lighting_intensity, TGAImage &tex) {
    float min_x = std::min(v0.X(), std::min(v1.X(), v2.X()));
    float min_y = std::min(v0.Y(), std::min(v1.Y(), v2.Y()));

    float max_x = std::max(v0.X(), std::max(v1.X(), v2.X()));
    float max_y = std::max(v0.Y(), std::max(v1.Y(), v2.Y()));

    // Could do binary search on x to see where the triangle starts if the image is bigger than say 64?
    for (int y = min_y; y <= max_y; y++) {
        bool drawing_triangle = false;

        for (int x = min_x; x <= max_x; x++) {
            Vec3f point(x, y);
            Vec3f barycentric = point.Barycentric(v0, v1, v2);

            if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0) {
                // If we are already drawing a triangle and we stepped out of it
                // we won't be drawing it again on this line.
                if (drawing_triangle)
                    break;

                continue;
            }
            drawing_triangle = true;

            float point_z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();
            float u = (barycentric.X() * uv0.X() + barycentric.Y() * uv1.X() + barycentric.Z() * uv2.X()) * tex.get_width();
            float v = (barycentric.X() * uv0.Y() + barycentric.Y() * uv1.Y() + barycentric.Z() * uv2.Y()) * tex.get_height();
            TGAColor tga_color = tex.get(u, v) * lighting_intensity;

            Color color(tga_color);

            int z_buf_index = ZBufIndex(x, y);
            if (point.Z() > z_buf->operator[](z_buf_index)) {
                z_buf->operator[](z_buf_index) = point_z;
                fb.Set(x, y, color);
            }
        }
    }
}

void Renderer::DrawModel(const Model &model, TGAImage &tex, Vec3f insertion_point, float scale) {
    Vec3f offset(400, 50, 0);
    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model.FaceCount(); i++) {
        std::vector<FaceIndices> *face = model.Face(i);
        Vec3f v0 = ((Vec3f(1, 1, 1) + model.Vert(face->operator[](0).vertex_index)) * scale)
            .TruncateCoeffs() + insertion_point;
        Vec3f v1 = ((Vec3f(1, 1, 1) + model.Vert(face->operator[](1).vertex_index)) * scale)
            .TruncateCoeffs() + insertion_point;
        Vec3f v2 = ((Vec3f(1, 1, 1) + model.Vert(face->operator[](2).vertex_index)) * scale)
            .TruncateCoeffs() + insertion_point;

        Vec3f uv0 = model.Uv(face->operator[](0).uv_index);
        Vec3f uv1 = model.Uv(face->operator[](1).uv_index);
        Vec3f uv2 = model.Uv(face->operator[](2).uv_index);

        // Simple lighting
        Vec3f triangle_normal = (v2 - v0).Cross(v1 - v0).Normalize();
        float lighting_intensity = triangle_normal.Dot(light_dir);
        if (lighting_intensity < 0) 
            continue;

        DrawTriangle(v0, v1, v2, uv0, uv1, uv2, lighting_intensity, tex);
    }
}
