#include <tuple>

#include "shaders/toon_shader.h"

ToonShader::ToonShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf, Color color, int shade_count) : 
    GouraudShader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf),
    color(color), shade_count(shade_count) {
    intensity_per_shade = 1.0 / shade_count;
}

std::tuple<bool, Color> ToonShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    float intensity = intensities.Dot(barycentric);
    bool zero_intensity = true;

    for (int i = shade_count; i > 0; i--) {
        if (intensity > intensity_per_shade * i) {
            intensity = intensity_per_shade * i;
            zero_intensity = false;
            break;
        }
    }

    if (zero_intensity)
        intensity = 0;

    return std::make_tuple(true, color * intensity);
}