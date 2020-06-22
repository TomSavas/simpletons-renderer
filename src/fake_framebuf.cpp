#include "fake_framebuf.h"

FakeFramebuf::FakeFramebuf(int width, int height) : width(width), height(height) {
    // Translate from unit cube into viewport
    Mat4f translation = Mat4f::Identity();
    translation[0][3] = (Width() - 1) / 2.0;
    translation[1][3] = (Height() - 1) / 2.0;
    translation[2][3] = 0.5;

    // Scale to viewport dimensions
    Mat4f scaling = Mat4f::Identity();
    scaling[0][0] = (Width() - 1) / 2.0;
    scaling[1][1] = (Height() - 1) / 2.0;
    scaling[2][2] = 0.5;

    viewport_matrix = translation * scaling;
}

int FakeFramebuf::Index(int x, int y) const {
    return 0;
}

int FakeFramebuf::Width() const {
    return width;
}

int FakeFramebuf::Height() const {
    return height;
}

int FakeFramebuf::Size() const {
    return width * height;
}

const Mat4f &FakeFramebuf::ViewportMatrix() const {
    return viewport_matrix;
}

void FakeFramebuf::Set(int x, int y, Color color) {
    return;
}

void FakeFramebuf::Clear(Color color) {
    return;
}

void FakeFramebuf::FlipY(bool flip) {
    return;
}

char& FakeFramebuf::operator[](int index) {
    return all_fb_values;
}