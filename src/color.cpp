#include "color.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : 
    r(r), g(g), b(b), a(a) {}

Color::Color(TGAColor &color) : 
    r(color[2]), g(color[1]), b(color[0]), a(color[3]) {}

Color Color::operator*(float rhs) const {
    return Color(r * rhs, g * rhs, b * rhs, a * rhs);
}