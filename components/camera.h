/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file camera.h

#ifndef SEELE_CAMERA_H
#define SEELE_CAMERA_H

#include "transform.h"

namespace components {

class Camera : public Transform {
public:
    Camera(const glm::vec3& look_from, const glm::vec3& look_at, const glm::vec3& vup, float fov, float width, float height);
    ~Camera() override;

    glm::vec4 transform(const glm::vec4& vec) const override;

    inline glm::vec4 transformNormal(const glm::vec4& n) const {
        return _rtv * n;
    }

    void rasterize(core::rasterization::RasterizationRenderer* renderer, int pass) override {}

    inline glm::vec2 getSize() const {
        return {_width, _height};
    }

    inline glm::vec3 getOrigin() const {
        return _look_from;
    }

    glm::vec3 getPixelPosition(float x, float y) const;

    bool intersect(const core::raytracing::Ray& ray, core::raytracing::HitRecord& record) const override { return false; }

    glm::vec3 calculateColor(core::raytracing::RaytracingRenderer* renderer, const core::raytracing::Ray& ray, float t) const override { return {}; }
private:
    glm::mat4 _v;
    glm::mat4 _p;
    glm::mat4 _rtv;

    glm::vec3 _look_from;
    glm::vec3 _look_at;
    glm::vec3 _vup;

    float _focus;
    float _width;
    float _height;
protected:
};

} // namespace components

#endif //SEELE_CAMERA_H
