#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <linux/fb.h>

#include "color.h"

#define FB_ERR_PREFIX "FRAMEBUF ERROR"

class Framebuf {
private:
    int fd = -1;
    char* fb = (char*) -1;
    struct fb_var_screeninfo var_info;
    struct fb_fix_screeninfo fix_info;
    int size;

    bool flip_y = true;

    void LoadFd();
    void LoadInfo();
    void MmapFb();

public:
    Framebuf();
    ~Framebuf();

    int Index(int x, int y) const;
    int Width() const;
    int Height() const;

    char *&Fb();
    void Set(int x, int y, Color color);
    void FlipY(bool flip);

    char& operator[](int index);
};

#endif
