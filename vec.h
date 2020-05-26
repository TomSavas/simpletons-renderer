#ifndef VEC_H
#define VEC_H

#include <string>

class Vec3f {
public:
    float x;
    float y;
    float z;

    Vec3f(float x = 0, float y = 0, float z = 0);

    Vec3f TruncateCoeffs() const;

    float SqrLen() const;
    float Len() const;

    float Dot(const Vec3f &rhs) const;
    Vec3f Cross(const Vec3f &rhs) const;
    Vec3f Normalize() const;

    // Calculates barycentric coordinates of the current point with respect to
    // triangle abc.
    //
    // Coordinates are [1-u-v, u, v] where
    // p = (1-u-v)a + u*b + v*c
    Vec3f Barycentric(const Vec3f &a, const Vec3f &b, const Vec3f &c) const;

    Vec3f operator-() const;
    Vec3f operator+(const Vec3f &rhs) const;
    Vec3f operator-(const Vec3f &rhs) const;
    Vec3f operator*(float rhs) const;

    std::string ToString() const;

    static Vec3f One();
};

#endif
