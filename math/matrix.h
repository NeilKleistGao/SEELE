/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file matrix.h

#ifndef SEELE_MATRIX_H
#define SEELE_MATRIX_H

#include <array>

#include "vector.h"

namespace math {
class Matrix {
public:
    Matrix();
    ~Matrix() = default;

    Matrix(const Matrix& other) {
        for (int i = 0; i < SIZE; ++i) {
            _mat[i] = other._mat[i];
        }
    }

    Matrix& operator= (const Matrix& other) {
        for (int i = 0; i < SIZE; ++i) {
            _mat[i] = other._mat[i];
        }
        return *this;
    }

    Matrix operator* (const Matrix& other);
    Matrix& operator*=(const Matrix& other);

    Vector operator* (const Vector& v);

    Matrix& move(const Vector& fwd);
    Matrix& scale(const Vector& scl);
    Matrix& rotate(const Vector& axis, const float& angle);

private:
    static constexpr size_t WIDTH = 4;
    static constexpr size_t SIZE = WIDTH*WIDTH;
    std::array<float, SIZE> _mat;
};
} // namespace math

#endif //SEELE_MATRIX_H
