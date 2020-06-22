#ifndef IFRAMEBUF_H
#define IFRAMEBUF_H

#include "matrix.hpp"

#include "color.h"

class IFramebuf {
public:
    virtual ~IFramebuf() = 0;

    virtual int Index(int x, int y) const = 0;
    virtual int Width() const = 0;
    virtual int Height() const = 0;
    virtual int Size() const = 0;
    virtual const Mat4f &ViewportMatrix() const = 0;

    virtual void Set(int x, int y, Color color) = 0;
    virtual void Clear(Color color = Color(0, 0, 0, 0)) = 0;

    virtual void FlipY(bool flip) = 0;

    virtual char& operator[](int index) = 0;
};

#endif