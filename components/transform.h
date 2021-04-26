/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file transform.h

#ifndef SEELE_TRANSFORM_H
#define SEELE_TRANSFORM_H

#include "glm/glm.hpp"

namespace core::rasterization {
class RasterizationRenderer;
} // namespace core::general

namespace components {
class Transform {
public:
    Transform();
    ~Transform() = default;

    void setPosition(const glm::vec3& position) {
        _position = position;
    }

    inline void setRotation(const glm::vec3& rotation) {
        _rotation = rotation;
    }

    inline void setScale(const glm::vec3& scale) {
        _scale = scale;
    }

    inline glm::vec3 getPosition() const {
        return _position;
    }

    inline glm::vec3 getRotation() const {
        return _rotation;
    }

    inline glm::vec3 getScale() const {
        return _scale;
    }

    inline glm::vec4 transform(const glm::vec4& v) {
        return _model * v;
    }

    virtual void rasterize(core::rasterization::RasterizationRenderer* renderer) = 0;
private:
    glm::mat4 _model;
protected:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    void updateModelMatrix();
};

} // namespace components

#endif //SEELE_TRANSFORM_H