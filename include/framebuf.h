#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <linux/fb.h>

#include "iframebuf.h"

#define FB_ERR_PREFIX "FRAMEBUF ERROR"

class Framebuf : public IFramebuf {
private:
    int fd = -1;
    char* fb = (char*) -1;
    struct fb_var_screeninfo var_info;
    struct fb_fix_screeninfo fix_info;
    int size_in_bytes;
    Mat4f viewport_matrix;

    bool flip_y = true;

    void LoadFd();
    void LoadInfo();
    void MmapFb();
    void CalculateViewportMatrix();

public:
    Framebuf();
    ~Framebuf();

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
