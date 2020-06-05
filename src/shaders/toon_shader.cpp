#include <tuple>

#include "shaders/toon_shader.h"

ToonShader::ToonShader(TGAImage &image, const Mat4f &mvp, const Vec3f &light_dir, Color color,
    int shade_count) : GouraudShader(image, mvp, light_dir), color(color), shade_count(shade_count) {
    intensity_per_shade = 1.0 / (shade_count - 1);
}

std::tuple<bool, Color> ToonShader::Fragment(const Vec3f &barycentric, const FaceInfo &face) {
    if (barycentric.X() < 0 || barycentric.Y() < 0 || barycentric.Z() < 0)
        return std::make_tuple(false, Color(0, 0, 0, 0));

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