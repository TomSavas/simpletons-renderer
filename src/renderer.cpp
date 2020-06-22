#include <algorithm>
#include <limits>
#include <stdlib.h>
#include <tuple>

#include "matrix.hpp"

#include "fake_framebuf.h"
#include "framebuf.h"
#include "renderer.h"
#include "shaders/empty_shader.h"
#include "shaders/flat_shader.h"
#include "shaders/gouraud_shader.h"
#include "shaders/normal_tex_shader.h"
#include "shaders/phong_shader.h"
#include "shaders/shader.h"
#include "shaders/toon_shader.h"
#include "shaders/tangent_normal_tex_shader.h"

Renderer::Renderer(const Mat4f &projection_mat, const Mat4f &view_mat, const Vec3f &camera_pos,
    const Vec3f &light_dir, const Mat4f &light_view_mat) : fb(), projection(projection_mat), view(view_mat),
    camera_pos(camera_pos), light_dir(light_dir), light_view(light_view_mat) {
    z_buf = new std::vector<float>(fb.Width() * fb.Height(), -std::numeric_limits<float>::max());
    shadow_buf = new std::vector<float>(fb.Width() * fb.Height(), -std::numeric_limits<float>::max());
}

Renderer::~Renderer() {
    delete z_buf;
    delete shadow_buf;
}

int Renderer::BufIndex(int x, int y) const {
    return x + y * fb.Width();
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

void Renderer::DrawLine(Vec4f v0, Vec4f v1, Color color) { 
    v0 = fb.ViewportMatrix() * (projection * view * v0).ProjectTo3d();
    v1 = fb.ViewportMatrix() * (projection * view * v1).ProjectTo3d();

    DrawLine(v0.X(), v0.Y(), v1.X(), v1.Y(), color);
}

void Renderer::DrawTriangle(const Model &model, Shader &shader, const FaceInfo &face,
    TGAImage &tex, std::vector<float> &depth_buf) {
        DrawTriangle(model, shader, face, tex, depth_buf, fb);
}

void Renderer::DrawTriangle(const Model &model, Shader &shader, const FaceInfo &face,
    TGAImage &tex, std::vector<float> &depth_buf, IFramebuf &framebuf) {
    Vec4f v0 = (framebuf.ViewportMatrix() * shader.Vertex(face, 0)).ProjectTo3d();
    Vec4f v1 = (framebuf.ViewportMatrix() * shader.Vertex(face, 1)).ProjectTo3d();
    Vec4f v2 = (framebuf.ViewportMatrix() * shader.Vertex(face, 2)).ProjectTo3d();

    float min_x = std::min(v0.X(), std::min(v1.X(), v2.X()));
    float min_y = std::min(v0.Y(), std::min(v1.Y(), v2.Y()));

    float max_x = std::max(v0.X(), std::max(v1.X(), v2.X()));
    float max_y = std::max(v0.Y(), std::max(v1.Y(), v2.Y()));

    for (int y = min_y; y <= max_y; y++) {
        // Ignore the point if it's out of viewport's bounds
        if (y < 0 || y >= framebuf.Height())
            continue;

        bool started_drawing_triangle = false;
        for (int x = min_x; x <= max_x; x++) {
            // Ignore the point if it's out of viewport's bounds
            if (x < 0 || x >= framebuf.Width())
                continue;

            Bary3f barycentric = Vec3f(x, y).Barycentric(v0.To<Vec3f, 3>(), v1.To<Vec3f, 3>(),
                v2.To<Vec3f, 3>());
            if (!barycentric.IsInsideTriangle()) {
                // If we were drawing a triangle on this line but are no more, then we can
                // just jump to another line, as there definitely won't be any more of this
                // triangle on this line
                if (started_drawing_triangle)
                    break;

                continue;
            }
            started_drawing_triangle = true;

            // Point is already in ndc, so we have already "projected into 3d",
            // hence we don't need division by w.
            float z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();

            int depth_buf_index = BufIndex(x, y);
            if (z < depth_buf[depth_buf_index])
                continue;

            bool should_render;
            Color color;
            std::tie(should_render, color) = shader.Fragment(barycentric, face);

            if (should_render) {
                depth_buf[depth_buf_index] = z;
                framebuf.Set(x, y, color);
            }
        }
    }
}

void Renderer::DrawModel(const Model &model, TGAImage &tex, TGAImage &normal_tex, const Mat4f &model_mat) {
    current_mvp = projection * view * model_mat;
    current_light_mvp = projection * light_view * model_mat;
    Mat4f camera_viewport_to_shadow_viewport_mat = fb.ViewportMatrix() * current_light_mvp * 
        (fb.ViewportMatrix() * current_mvp).Inverse();
    FakeFramebuf fake_fb(fb.Width(), fb.Height());

    Shader *empty_shader = new EmptyShader(tex, current_light_mvp, Mat4f::Identity(), light_dir,
        fake_fb, *shadow_buf);
    //Shader *shader = new FlatShader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat,
    //    light_dir, fb, *shadow_buf);
    //Shader *shader = new GouraudShader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat,
    //    light_dir, fb, *shadow_buf);
    //Shader *shader = new NormalTexShader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat,
    //    light_dir, fb, *shadow_buf, normal_tex);
    //Shader *shader = new PhongShader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat,
    //    light_dir, fb, *shadow_buf);
    //Shader *shader = new Shader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat, light_dir,
    //    fb, *shadow_buf);
    //Shader *shader = new ToonShader(tex, current_mvp, camera_viewport_to_shadow_viewport_mat,
    //    light_dir, fb, *shadow_buf, Color(148, 98, 91), 5);
    Shader *shader = new TangentNormalTexShader(tex, current_mvp,
        camera_viewport_to_shadow_viewport_mat, light_dir, fb, *shadow_buf, normal_tex);

    // Render from light's perspective in order to produce shadow_buf
    // We dump the render results into a non-existant framebuffer
    for (int i = 0; i < model.FaceCount(); i++)
        DrawTriangle(model, *empty_shader, model.Face(i), tex, *shadow_buf, fake_fb);

    // Actual rendering
    for (int i = 0; i < model.FaceCount(); i++)
        DrawTriangle(model, *shader, model.Face(i), tex, *z_buf);

    delete empty_shader;
    delete shader;
}

void Renderer::DrawAxes() {
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0.25, 0, 0, 1), Color(255, 0, 0, 0));
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0, 0.25, 0, 1), Color(0, 255, 0, 0));
    DrawLine(Vec4f(0, 0, 0, 1), Vec4f(0, 0, 0.25, 1), Color(0, 0, 255, 0));
}