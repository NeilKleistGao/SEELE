/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file ray.h

#ifndef SEELE_RAY_H
#define SEELE_RAY_H

#include "glm/glm.hpp"

namespace core::raytracing {

class Ray {
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    ~Ray() = default;

    inline glm::vec3 getOrigin() const {
        return _origin;
    }

    inline glm::vec3 getDirection() const {
        return _direction;
    }

    inline glm::vec3 at(float t) const {
        return _origin + t * _direction;
    }
private:
    glm::vec3 _origin;
    glm::vec3 _direction;
};

} // namespace raytracing

#endif //SEELE_RAY_H
