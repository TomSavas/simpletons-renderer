#include "shaders/normal_tex_shader.h"

NormalTexShader::NormalTexShader(TGAImage &tex, const Mat4f &mvp,
    const Mat4f &camera_viewport_to_shadow_viewport, const Vec3f &light_dir,
    const IFramebuf &framebuf, const std::vector<float> &shadow_buf, TGAImage &normal_tex) : 
    Shader(tex, mvp, camera_viewport_to_shadow_viewport, light_dir, framebuf, shadow_buf),
    normal_tex(normal_tex) {}

std::tuple<bool, Color> NormalTexShader::Fragment(const Bary3f &barycentric, const FaceInfo &face) {
    Vec2f uv = Uv(barycentric, face);
    TGAColor normal_as_color = normal_tex.get(uv.X(), uv.Y());

    Vec3f normal((float)normal_as_color[2] - 255.0/2.0 , (float)normal_as_color[1] - 255.0/2.0,
        (float)normal_as_color[0] - 255.0/2.0);
    normal = normal.Normalize();
    normal = (mvp_inverse_transpose * normal.To<Vec4f, 4>()).To<Vec3f, 3>().Normalize();

    float intensity = normal.Dot(-light_dir_in_camera_space) - HardShadowIntensity(barycentric);

    TGAColor tex_color = tex.get(uv.X(), uv.Y()) * intensity;
    return std::make_tuple(true, Color(tex_color));
}