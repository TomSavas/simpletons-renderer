#include <stdlib.h>
#include <algorithm>

#include "renderer.h"

Renderer::Renderer() : fb() {}

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
    DrawLine(v0.x, v0.y, v1.x, v1.y, color);
}

void Renderer::DrawTriangle(Vec3f v0, Vec3f v1, Vec3f v2, Color color) {
    float min_x = std::min(v0.x, std::min(v1.x, v2.x));
    float min_y = std::min(v0.y, std::min(v1.y, v2.y));

    float max_x = std::max(v0.x, std::max(v1.x, v2.x));
    float max_y = std::max(v0.y, std::max(v1.y, v2.y));

    // Could do binary search on x to see where the triangle starts if the image is bigger than say 64?
    for (int y = min_y; y <= max_y; y++) {
        bool drawing_triangle = false;

        for (int x = min_x; x <= max_x; x++) {
            Vec3f point(x, y);
            Vec3f barycentric = point.Barycentric(v0, v1, v2);

            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
                // If we are already drawing a triangle and we stepped out of it
                // we won't be drawing it again on this line.
                if (drawing_triangle)
                    break;

                continue;
            }
            drawing_triangle = true;

            fb.Set(x, y, color);
        }
    }
}

void Renderer::DrawModel(const Model &model, Vec3f insertion_point, float scale) {
    Vec3f offset = Vec3f(400, 50, 0);
    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model.FaceCount(); i++) {
        std::vector<int> *face = model.Face(i);
        Vec3f v0 = ((Vec3f::One() + model.Vert(face->operator[](0))) * scale).TruncateCoeffs() + insertion_point;
        Vec3f v1 = ((Vec3f::One() + model.Vert(face->operator[](1))) * scale).TruncateCoeffs() + insertion_point;
        Vec3f v2 = ((Vec3f::One() + model.Vert(face->operator[](2))) * scale).TruncateCoeffs() + insertion_point;

        // Simple lighting
        Vec3f triangle_normal = (v2 - v0).Cross(v1 - v0).Normalize();
        float lighting_intensity = triangle_normal.Dot(light_dir);
        if (lighting_intensity < 0) continue;
        Color color(lighting_intensity * 255, lighting_intensity * 255, lighting_intensity * 255, 0);

        DrawTriangle(v0, v1, v2, color);
    }
}
