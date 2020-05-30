#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <linux/fb.h>

#include "color.h"
#include "matrix.h"

#define FB_ERR_PREFIX "FRAMEBUF ERROR"

class Framebuf {
private:
    int fd = -1;
    char* fb = (char*) -1;
    struct fb_var_screeninfo var_info;
    struct fb_fix_screeninfo fix_info;
    int size;
    Mat4f viewport_matrix;

    bool flip_y = true;

    void LoadFd();
    void LoadInfo();
    void MmapFb();
    void CalculateViewportMatrix();

public:
    Framebuf();
    ~Framebuf();

    int Index(int x, int y) const;
    int Width() const;
    int Height() const;
    const Mat4f &ViewportMatrix() const;

    char *&Fb();
    void FlipY(bool flip);
    void Set(int x, int y, Color color);
    void Clear(Color color = Color(0, 0, 0, 0));

    char& operator[](int index);
};

#endif
