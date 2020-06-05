#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <stdexcept>

#include "vec.hpp"

template<int Y_DIM, int X_DIM> class Matrix;

template<int DIM>
struct Dt {
    static float Determinant(const Matrix<DIM, DIM> *mat);
};

template<int Y_DIM, int X_DIM>
class Matrix {
private:
    float values[Y_DIM][X_DIM];   

public:
    Matrix() {}

    Matrix(float initial_values[Y_DIM][X_DIM]) {
        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                values[y][x] = initial_values[y][x];
    }

    Matrix<X_DIM, Y_DIM> Transpose() const {
        Matrix<X_DIM, Y_DIM> transposed_mat;

        for (int x = 0; x < X_DIM; x++)
            for (int y = 0; y < Y_DIM; y++)
                transposed_mat[x][y] = values[y][x];

        return transposed_mat;
    }

    Matrix<Y_DIM - 1, X_DIM - 1> Minor(int row, int column) const {
        Matrix<Y_DIM - 1, X_DIM - 1> minor;

        for (int y = 0; y < Y_DIM - 1; y++)
            for (int x = 0; x < X_DIM - 1; x++)
                minor[y][x] = values[y >= row ? y + 1 : y][x >= column ? x + 1 : x];

        return minor;
    }

    float Cofactor(int row, int column) const {
        return Minor(row, column).Determinant() * ((row + column) % 2 == 0 ? 1 : -1);
    }   

    float Determinant() const {
        static_assert(Y_DIM == X_DIM, "Can only calculate determinant of a square matrix");

        return Dt<Y_DIM>::Determinant(this);
    }

    Matrix<Y_DIM, X_DIM> CofactorMatrix() const {
        Matrix<Y_DIM, X_DIM> cofactor_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                cofactor_mat[y][x] = Cofactor(y, x);

        return cofactor_mat;
    }   

    Matrix<X_DIM, Y_DIM> Adjugate() const {
        return CofactorMatrix().Transpose();
    }

    Matrix<Y_DIM, X_DIM> Inverse() const {
        static_assert(Y_DIM == X_DIM, "Can only calculate transpose of a square matrix");

        return Adjugate() * (1.0 / Determinant());
    }

    template<int OTHER_X_DIM>
    Matrix<Y_DIM, OTHER_X_DIM> operator*(const Matrix<X_DIM, OTHER_X_DIM> &rhs) const {
        Matrix<Y_DIM, OTHER_X_DIM> multiplied_mat;

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

    Matrix<Y_DIM, X_DIM> operator+(const Matrix<Y_DIM, X_DIM> &rhs) const {
        Matrix<X_DIM, Y_DIM> sum_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                sum_mat[y][x] = values[y][x] + rhs[y][x];

        return sum_mat;
    }

    Matrix<Y_DIM, X_DIM> operator-(const Matrix<Y_DIM, X_DIM> &rhs) const {
        Matrix<X_DIM, Y_DIM> diff_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                diff_mat[y][x] = values[y][x] - rhs[y][x];

        return diff_mat;
    }

    Matrix<Y_DIM, X_DIM> operator*(float rhs) const {
        Matrix<X_DIM, Y_DIM> scaled_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                scaled_mat[y][x] = values[y][x] * rhs;

        return scaled_mat;
    }

    template<typename VecImpl>
    VecImpl operator*(const VecBase<VecImpl, X_DIM, float> &rhs) const {
        VecImpl vec;

        for (int y = 0; y < X_DIM; y++) {
            vec[y] = 0;
            for (int x = 0; x < Y_DIM; x++)
                vec[y] += values[y][x] * rhs[x];
        }

        return vec;
    }

    auto operator[](int index) -> float (&)[X_DIM]{
        return values[index];
    }

    auto operator[](int index) const -> const float (&)[X_DIM] {
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

    static Matrix<Y_DIM, X_DIM> Identity() {
        Matrix<Y_DIM, X_DIM> identity_mat;

        for (int y = 0; y < Y_DIM; y++)
            for (int x = 0; x < X_DIM; x++)
                identity_mat[y][x] = x == y ? 1 : 0;

        return identity_mat;
    }
};

template<int DIM>
float Dt<DIM>::Determinant(const Matrix<DIM, DIM> *mat) {
    float determinant = 0;
    for (int i = 0; i < DIM; i++)
        determinant += mat->operator[](0)[i] * mat->Cofactor(0, i);

    return determinant;
}

template<>
struct Dt<2> {
    static float Determinant(const Matrix<2, 2> *mat) {
        return mat->operator[](0)[0] * mat->operator[](1)[1] - mat->operator[](0)[1] * mat->operator[](1)[0];
    }
};

template<>
struct Dt<3> {
    static float Determinant(const Matrix<3, 3> *mat) {
        return mat->operator[](0)[0] * mat->operator[](1)[1] * mat->operator[](2)[2] + 
                mat->operator[](0)[1] * mat->operator[](1)[2] * mat->operator[](2)[0] +
                mat->operator[](0)[2] * mat->operator[](1)[0] * mat->operator[](2)[1] -

                mat->operator[](0)[2] * mat->operator[](1)[1] * mat->operator[](2)[0] -
                mat->operator[](0)[1] * mat->operator[](1)[0] * mat->operator[](2)[2] -
                mat->operator[](0)[0] * mat->operator[](1)[2] * mat->operator[](2)[1];
    }
};

using Mat4f = Matrix<4, 4>;

#endif
