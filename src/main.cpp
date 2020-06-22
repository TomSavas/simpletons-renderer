#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"

Mat4f ProjectionMat(Vec3f camera) {
    Mat4f mat = Mat4f::Identity();
    mat[3][2] = -1.0 / camera.Z();

    return mat;
}

Mat4f LookAt(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f forward = (center - eye).Normalize();
    Vec3f side = forward.Cross(up).Normalize();
    // Recalculate up to make sure that all vectors are orthogonal to each other
    up = side.Cross(forward).Normalize();

    Mat4f new_basis_vectors_mat = Mat4f::Identity();
    new_basis_vectors_mat[0][0] = side.X();
    new_basis_vectors_mat[0][1] = side.Y();
    new_basis_vectors_mat[0][2] = side.Z();

    new_basis_vectors_mat[1][0] = up.X();
    new_basis_vectors_mat[1][1] = up.Y();
    new_basis_vectors_mat[1][2] = up.Z();

    // We're in a right-hand coordinate system, z axis comes towards us
    new_basis_vectors_mat[2][0] = -forward.X();
    new_basis_vectors_mat[2][1] = -forward.Y();
    new_basis_vectors_mat[2][2] = -forward.Z();

    Mat4f translation_mat = Mat4f::Identity();
    translation_mat[3][0] = -center.X();
    translation_mat[3][1] = -center.Y();
    translation_mat[3][2] = center.Z();

    // This is rather simple, we're simply translating to our new coordinate space and then
    // applying our new basis vectors which are given by side, up and forward vectors 
    // (x, y, z respectively)
    return new_basis_vectors_mat * translation_mat;
}

int main() {
    Model model("./model.obj");

    TGAImage tex;
    tex.read_tga_file("./diffuse.tga");
    tex.flip_vertically();

    TGAImage tangent_normal_tex;
    tangent_normal_tex.read_tga_file("./nm_tangent.tga");
    tangent_normal_tex.flip_vertically();

    Vec3f light_dir = -Vec3f(1, 0, 1);
    Vec3f camera_pos = Vec3f(0.6, 1.1, 3);

    Mat4f projection_mat = ProjectionMat(camera_pos);

    Mat4f view_mat = LookAt(camera_pos, Vec3f(), Vec3f(0, 1));
    Mat4f light_view_mat = LookAt(-light_dir, Vec3f(), Vec3f(0, 1));

    Renderer renderer(projection_mat, view_mat, camera_pos, light_dir, light_view_mat);

    // I'm on a 16:9 monitor, so let's resize the model to look normaly on that kind of a display
    Mat4f model_mat = Mat4f::Identity();
    model_mat[0][0] = 9.0/16.0 * 0.8;
    model_mat[1][1] = 0.8;
    model_mat[2][2] = 0.8;

    renderer.DrawModel(model, tex, tangent_normal_tex, model_mat);
    renderer.DrawAxes();
    getchar();

    return 0;
}
