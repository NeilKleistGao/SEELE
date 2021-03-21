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

#ifndef SEELE_VECTOR_H
#define SEELE_VECTOR_H

#include "utils/registration.h"

namespace math {

class Vector {
public:
    float x, y, z, w;

    static const Vector X, Y, Z;

    Vector(const float& x, const float& y);
    explicit Vector(const float& x = 0, const float& y = 0, const float& z = 0, const float& w = 1);

    Vector& operator+= (const Vector& other);

    inline Vector operator+ (const Vector& other) const {
        Vector v{x, y, z, w};
        v += other;
        return v;
    }

    Vector& operator^= (const Vector& other);

    inline Vector operator^ (const Vector& other) const {
        Vector v{x, y, z, w};
        v ^= other;
        return v;
    }

    inline Vector& operator-= (const Vector& other) {
        return (*this) += (-other);
    }

    inline Vector operator- (const Vector& other) const {
        Vector v{x, y, z, w};
        v -= other;
        return v;
    }

    inline Vector operator- () const {
        Vector v{-x, -y, -z, -w};
        return v;
    }

    Vector& operator*= (const float& f);
    inline Vector operator* (const float& f) const {
        Vector v{x * f, y * f, z * f, w * f};
        return v;
    }

private:
};

inline Vector operator* (const float& f, const Vector& v) {
    return v * f;
}

SEELE_REGISTRATION(Vector) {
    luabridge::getGlobalNamespace(script::RenderingScript::getInstance()->getState())
        .beginNamespace("seele")
            .beginClass<Vector>("Vector")
                .addProperty("x", &Vector::x, true)
                .addProperty("y", &Vector::y, true)
                .addProperty("z", &Vector::z, true)
                .addConstructor<void (*) (const float &, const float &, const float &)>()
            .endClass()
        .endNamespace();
}

} // namespace math

#endif //SEELE_VECTOR_H
