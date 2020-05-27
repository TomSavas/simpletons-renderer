#ifndef COLOR_H
#define COLOR_H

#include "tgaimage.h"

typedef struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
        r(r), g(g), b(b), a(a) {}

    Color(TGAColor &color) : 
        r(color[2]), g(color[1]), b(color[0]), a(color[3]) {}
} Color;

#endif 
