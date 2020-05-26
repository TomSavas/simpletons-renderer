#include <math.h>

#include "vec.h"

Vec3f::Vec3f(float x, float y, float z): x(x), y(y), z(z) {}

Vec3f Vec3f::TruncateCoeffs() const {
    return Vec3f((int)x, (int)y, (int)z);
}

float Vec3f::SqrLen() const {
    return x * x + y * y + z * z;
}

float Vec3f::Len() const {
    return sqrt(SqrLen());
}

float Vec3f::Dot(const Vec3f &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec3f Vec3f::Cross(const Vec3f &rhs) const {
    return Vec3f(y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x);
}

Vec3f Vec3f::Normalize() const {
    float len = Len();

    return Vec3f(x / len, y / len, z / len);
}

Vec3f Vec3f::Barycentric(const Vec3f &a, const Vec3f &b, const Vec3f &c) const {
    Vec3f pa = a - *this;
    Vec3f ab = b - a;
    Vec3f ac = c - a;

    // [u, v, 1]
    Vec3f barycentric = Vec3f(ab.x, ac.x, pa.x).Cross(Vec3f(ab.y, ac.y, pa.y));

    return Vec3f((1 - barycentric.x - barycentric.y) / barycentric.z,
            barycentric.x / barycentric.z,
            barycentric.y / barycentric.z);
}

Vec3f Vec3f::operator-() const {
    return Vec3f(-x, -y, -z);
}

Vec3f Vec3f::operator+(const Vec3f& rhs) const {
    return Vec3f(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3f Vec3f::operator-(const Vec3f& rhs) const {
    return Vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3f Vec3f::operator*(float rhs) const {
    return Vec3f(rhs * x, rhs * y, rhs * z);
}

std::string Vec3f::ToString() const {
    return "x: " + std::to_string(x) + 
        ", y: " + std::to_string(y) + 
        ", z: " + std::to_string(z);
}

Vec3f Vec3f::One() {
    return Vec3f(1, 1, 1);
}
