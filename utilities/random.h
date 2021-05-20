/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file random.h

#ifndef SEELE_RANDOM_H
#define SEELE_RANDOM_H

#include <random>

namespace utilities {

class Random {
public:
    Random() : _distribution(0.0f, 1.0f), _generator() {}
    ~Random() = default;

    inline float roll() {
        return _distribution(_generator);
    }

    inline float roll(float min, float max) {
        float r = roll();
        return min + (max - min) * r;
    }
private:
    std::uniform_real_distribution<float> _distribution;
    std::mt19937 _generator;
};

} // namespace utilities

#endif //SEELE_RANDOM_H
