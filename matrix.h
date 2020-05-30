#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <stdexcept>

#include "vec.h"

template<int Y_DIM, int X_DIM, typename T>
class Matrix {
private:
    T values[Y_DIM][X_DIM];   

public:
    Matrix() {}

    Matrix(T initial_values[Y_DIM][X_DIM]) {
        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                values[y][x] = initial_values[y][x];
    }

    Matrix<X_DIM, Y_DIM, T> Transpose() const {
        Matrix<X_DIM, Y_DIM, T> transposed_mat;

        for (int x = 0; x < X_DIM; x++)
            for (int y = 0; y < Y_DIM; y++)
                transposed_mat[x][y] = values[y][x];

        return transposed_mat;
    }

    template<int OTHER_X_DIM>
    Matrix<Y_DIM, OTHER_X_DIM, T> operator*(const Matrix<X_DIM, OTHER_X_DIM, T> &rhs) const {
        Matrix<Y_DIM, OTHER_X_DIM, T> multiplied_mat;

        for (int y = 0; y < Y_DIM; y++) {
            for (int x = 0; x < OTHER_X_DIM; x++) {
                float scalar = 0;

                for (int i = 0; i < X_DIM; i++)
                    scalar += values[y][i] * rhs[i][x];

                multiplied_mat[y][x] = scalar;
            }
        }

        return multiplied_mat;
    }

    Matrix<Y_DIM, X_DIM, T> operator+(const Matrix<Y_DIM, X_DIM, T> &rhs) const {
        Matrix<X_DIM, Y_DIM, T> sum_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                sum_mat[y][x] = values[y][x] + rhs[y][x];

        return sum_mat;
    }

    Matrix<Y_DIM, X_DIM, T> operator-(const Matrix<Y_DIM, X_DIM, T> &rhs) const {
        Matrix<X_DIM, Y_DIM, T> diff_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                diff_mat[y][x] = values[y][x] - rhs[y][x];

        return diff_mat;
    }

    Matrix<Y_DIM, X_DIM, T> operator*(T rhs) const {
        Matrix<X_DIM, Y_DIM, T> scaled_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                scaled_mat[y][x] = values[y][x] * rhs;

        return scaled_mat;
    }

    template<typename VecImpl>
    VecImpl operator*(const VecBase<VecImpl, X_DIM, T> &rhs) const {
        VecImpl vec;

        for (int y = 0; y < X_DIM; y++) {
            vec[y] = 0;
            for (int x = 0; x < Y_DIM; x++)
                vec[y] += values[y][x] * rhs[x];
        }

        return vec;
    }

    auto operator[](int index) -> T (&)[X_DIM]{
        return values[index];
    }

    auto operator[](int index) const -> const T (&)[X_DIM] {
        return values[index];
    }

    std::string ToString() const {
        std::string str = "---\n";

        for (int y = 0; y < Y_DIM; y++) {
            str += "{ ";
            for (int x = 0; x < X_DIM; x++)
                str += std::to_string(values[y][x]) + ", ";
            str += "},\n" ;
        }

        str += "---\n";

        return str;
    }

    static Matrix<Y_DIM, X_DIM, T> Identity() {
        Matrix<Y_DIM, X_DIM, T> identity_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                identity_mat[y][x] = x == y ? 1 : 0;

        return identity_mat;
    }
};

using Mat4f = Matrix<4, 4, float>;

#endif
