#ifndef COLOR_H
#define COLOR_H

#include "tgaimage.h"

typedef struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 0);
    Color(TGAColor &color);

    Color operator*(float rhs) const;
} Color;

#endif 
