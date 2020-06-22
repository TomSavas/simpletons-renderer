#ifndef FAKE_FRAMEBUF_H
#define FAKE_FRAMEBUF_H

#include "iframebuf.h"

class FakeFramebuf : public IFramebuf {
private:
    int width;
    int height;

    Mat4f viewport_matrix;
    char all_fb_values = 0;

public:
    FakeFramebuf(int width, int height);

    int Index(int x, int y) const override;
    int Width() const override;
    int Height() const override;
    int Size() const override;
    const Mat4f &ViewportMatrix() const override;

    void Set(int x, int y, Color color) override;
    void Clear(Color color = Color(0, 0, 0, 0)) override;

    void FlipY(bool flip) override;

    char& operator[](int index) override;
};

#endif