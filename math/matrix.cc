/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file matrix.cc

#include "matrix.h"

namespace math {

Matrix::Matrix() {
    for (int i = 0; i < SIZE; ++i) {
        _mat[i] = (i % 5 == 0);
    }
}

Matrix Matrix::operator*(const Matrix& other) {
    Matrix res{};
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            res._mat[i * WIDTH + j] = 0;
            for (int k = 0; k < WIDTH; ++k) {
                res._mat[i * WIDTH + j] += _mat[i * WIDTH + k] * other._mat[k * WIDTH + j];
            }
        }
    }

    return res;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    std::array<int, SIZE> res{};
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            res[i * WIDTH + j] = 0;
            for (int k = 0; k < WIDTH; ++k) {
                res[i * WIDTH + j] += _mat[i * WIDTH + k] * other._mat[k * WIDTH + j];
            }
        }
    }

    for (int i = 0; i < SIZE; ++i) {
        _mat[i] = res[i];
    }
    return *this;
}

Vector Matrix::operator* (const Vector& v) {
    Vector res{v.x * _mat[0] + v.y * _mat[1] + v.z * _mat[2] + v.w * _mat[3],
               v.x * _mat[4] + v.y * _mat[5] + v.z * _mat[6] + v.w * _mat[7],
               v.x * _mat[8] + v.y * _mat[9] + v.z * _mat[10] + v.w * _mat[11],
               v.x * _mat[12] + v.y * _mat[13] + v.z * _mat[14] + v.w * _mat[15]};
    return res;
}

Matrix& Matrix::move(const Vector& fwd) {
    Matrix m{};
    m._mat[3] = fwd.x; m._mat[7] = fwd.y; m._mat[11] = fwd.z;
    std::swap(m, *this);
    return (*this)*=m;
}

Matrix& Matrix::scale(const Vector& scl) {
    Matrix m{};
    m._mat[0] = scl.x; m._mat[5] = scl.y; m._mat[10] = scl.z; m._mat[15] = scl.w;
    std::swap(m, *this);
    return (*this)*=m;
}

Matrix& Matrix::rotate(const Vector& axis, const float& angle) {
    Matrix m{};
    float s = std::sin(angle), c = std::cos(angle);

    if (axis == Vector::X) {
        m._mat[5] = m._mat[10] = c;
        m._mat[6] = -s; m._mat[9] = s;
    }
    else if (axis == Vector::Y) {
        m._mat[0] = m._mat[10] = c;
        m._mat[8] = -s; m._mat[2] = s;
    }
    else if (axis == Vector::Z) {
        m._mat[0] = m._mat[5] = c;
        m._mat[1] = -s; m._mat[4] = s;
    }
    else {
        utils::Debug::logErr("invalid rotating axis!");
        return *this;
    }

    std::swap(m, *this);
    return (*this)*=m;
}

} // namespace math