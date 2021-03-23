/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file vector.h

#include "vector.h"

namespace math {

const Vector Vector::X = Vector{1, 0, 0, 0};
const Vector Vector::Y = Vector{0,1, 0, 0};
const Vector Vector::Z = Vector{0, 0, 1, 0};

Vector::Vector(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {
}

Vector& Vector::operator+= (const Vector& other) {
    x += other.x; y += other.y; z += other.z; w += other.w;
    return *this;
}

Vector& Vector::operator^= (const Vector& other) {
    float tx = y * other.z - z * other.y,
          ty = other.x * z - x * other.z,
          tz = x * other.y - y * other.z;
    x = tx, y = ty, z = tz;
    return *this;
}

Vector& Vector::operator*= (const float& f) {
    x *= f; y *= f; z *= f; w *= f;
    return *this;
}

} // namespace math