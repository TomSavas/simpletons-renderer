#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <string>

template<typename VecImpl, int N, typename T>
class VecBase {
protected:
    T values[N];

public:
    VecBase() {}

    VecBase(T values[N]) {
        for (int i = 0; i < N; i++)
            this->values[i] = values[i];
    }

    VecImpl TruncateCoeffs() const {
        VecImpl truncated_vec;

        for (int i = 0; i < N; i++)
            truncated_vec[i] = (int) values[i];

        return VecImpl(truncated_vec);
    }

    float SqrLen() const {
        float sqr_len = 0;
    
        for (int i = 0; i < N; i++)
            sqr_len += values[i] * values[i];

        return sqr_len;
    }

    float Len() const {
        return sqrt(SqrLen());
    }

    float Dot(const VecBase<VecImpl, N, T> &rhs) const {
        float dot_product = 0;
        
        for (int i = 0; i < N; i++)
            dot_product += values[i] * rhs[i];

        return dot_product;
    }

    VecImpl Normalize() const {
        float len = Len();

        VecImpl normalized_vec;

        for (int i = 0; i < N; i++)
            normalized_vec[i] = values[i] / len;

        return normalized_vec;
    }

    template<typename OtherVecImpl, int M>
    OtherVecImpl To(T fill_value = 1) const {
        OtherVecImpl other_vec;

        int min = std::min(M, N);
        for (int i = 0; i < min; i++)
            other_vec[i] = values[i];

        for (int i = min; i < M; i++) 
            other_vec[i] = fill_value;

        return other_vec;
    }

    std::string ToString() const {
        std::string str;

        for (int i = 0; i < N - 1; i++)
            str += std::to_string(i) + ": " + std::to_string(values[i]) + ", ";
        str += std::to_string(N - 1) + ": " + std::to_string(values[N - 1]);

        return str;
    }

    VecImpl operator-() const {
        VecImpl inverted_vec;   

        for (int i = 0; i < N; i++) 
            inverted_vec[i] = -values[i];

        return inverted_vec;
    }

    VecImpl operator+(const VecImpl &rhs) const {
        VecImpl sum_vec;   

        for (int i = 0; i < N; i++) 
            sum_vec[i] = values[i] + rhs[i];

        return sum_vec;
        
    }

    VecImpl operator-(const VecImpl &rhs) const {
        VecImpl diff_vec;   

        for (int i = 0; i < N; i++) 
            diff_vec[i] = values[i] - rhs[i];

        return diff_vec;
        
    }

    VecImpl operator*(float rhs) const {
        VecImpl scaled_vec;   

        for (int i = 0; i < N; i++) 
            scaled_vec[i] = values[i] * rhs;

        return scaled_vec;
        
    }

    float &operator[](int index) {
        return values[index];  
    }

    const float &operator[](int index) const {
        return values[index];  
    }
};

template<typename T>
class Vec2 : public VecBase<Vec2<T>, 2, T> {
public:
    Vec2(T values[2]) : VecBase<Vec2<T>, 2, T>(values) {}

    Vec2(T x = 0, T y = 0) {
        this->values[0] = x;
        this->values[1] = y;
    };

    T &X() { return this->values[0]; }
    const T &X() const { return this->values[0]; }

    T &Y() { return this->values[1]; }
    const T &Y() const { return this->values[1]; }
};

template<typename T>
class BarycentricCoords3;

template<typename T>
class Vec3 : public VecBase<Vec3<T>, 3, T> {
public:
    Vec3(T values[3]) : VecBase<Vec3<T>, 3, T>(values) {}

    Vec3(T x = 0, T y = 0, T z = 0) {
        this->values[0] = x;
        this->values[1] = y;
        this->values[2] = z;
    };

    T &X() { return this->values[0]; }
    const T &X() const { return this->values[0]; }

    T &Y() { return this->values[1]; }
    const T &Y() const { return this->values[1]; }

    T &Z() { return this->values[2]; }
    const T &Z() const { return this->values[2]; }

    // Calculates barycentric coordinates of the current point with respect to
    // triangle abc.
    //
    // Coordinates are [1-u-v, u, v] where
    // p = (1-u-v)a + u*b + v*c
    BarycentricCoords3<T> Barycentric(const Vec3<T> &a, const Vec3<T> &b, const Vec3<T> &c) const {
        Vec3<T> pa = a - *this;
        Vec3<T> ab = b - a;
        Vec3<T> ac = c - a;

        // [u, v, 1]
        Vec3<T> barycentric = Vec3(ab.X(), ac.X(), pa.X()).Cross(Vec3<T>(ab.Y(), ac.Y(), pa.Y()));

        return BarycentricCoords3<T>(1.0 - (barycentric.X() + barycentric.Y()) / barycentric.Z(),
                barycentric.X() / barycentric.Z(),
                barycentric.Y() / barycentric.Z());
    }

    Vec3<T> Cross(const Vec3<T> &rhs) const {
        return Vec3(Y() * rhs.Z() - Z() * rhs.Y(),
            Z() * rhs.X() - X() * rhs.Z(),
            X() * rhs.Y() - Y() * rhs.X());
    }

};

template<typename T>
class BarycentricCoords3 : public Vec3<T> {
public:
    BarycentricCoords3(T values[3]) : Vec3<T>(values) {}

    BarycentricCoords3(T x = 0, T y = 0, T z = 0) : Vec3<T>(x, y, z) {}

    template<typename TVecImpl, int N>
    TVecImpl ApplyOn(const VecBase<TVecImpl, N, T> &a, const VecBase<TVecImpl, N, T> &b, const VecBase<TVecImpl, N, T> &c) const {
        TVecImpl result_vec;

        for (int i = 0; i < N; i++)
            result_vec[i] = this->X() * a[i] + this->Y() * b[i] + this->Z() * c[i];

        return result_vec;
    }

    template<typename TVecImpl, int N>
    TVecImpl ApplyOn(const VecBase<TVecImpl, N, T> vecs[3]) const {
        return ApplyOn<TVecImpl, N>(vecs[0], vecs[1], vecs[2]);
    }
    //Vec3<T> ApplyOn(const Vec3<T> &a, const Vec3<T> &b, const Vec3<T> &c) const {
    //    return Vec3<T>(this->X() * a.X() + this->Y() * b.X() + this->Z() * c.X(),
    //                this->X() * a.Y() + this->Y() * b.Y() + this->Z() * c.Y(),
    //                this->X() * a.Z() + this->Y() * b.Z() + this->Z() * c.Z());
    //}

    //Vec3<T> ApplyOn(Vec3<T> vecs[3]) const {
    //    return ApplyOn(vecs[0], vecs[1], vecs[2]);
    //}

    bool IsInsideTriangle() const {
        return this->X() >= 0 && this->Y() >= 0 && this->Z() >= 0;
    }
};

template<typename T>
class Vec4 : public VecBase<Vec4<T>, 4, T> {
public:
    Vec4(T values[4]) : VecBase<Vec4<T>, 4, T>(values) {}

    Vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
        this->values[0] = x;
        this->values[1] = y;
        this->values[2] = z;
        this->values[3] = w;
    };

    Vec4<T> ProjectTo3d() const {
        return Vec4<T>(X() / W(), Y() / W(), Z() / W(), 1);
    }

    T &X() { return this->values[0]; }
    const T &X() const { return this->values[0]; }

    T &Y() { return this->values[1]; }
    const T &Y() const { return this->values[1]; }

    T &Z() { return this->values[2]; }
    const T &Z() const { return this->values[2]; }

    T &W() { return this->values[3]; }
    const T &W() const { return this->values[3]; }
};

using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;
using Vec4f = Vec4<float>;

using Bary3f = BarycentricCoords3<float>;

#endif
