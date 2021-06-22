/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file light.h

#ifndef SEELE_LIGHT_H
#define SEELE_LIGHT_H

#include "transform.h"

namespace components {

class Light : public Transform {
public:
    Light() : Transform() {}
    ~Light() override = default;

    struct LightData {
        glm::vec3 direction;
        glm::vec3 color;

        LightData() : direction(0, 0, 0), color(0, 0, 0) {}
    };

    virtual LightData getLightData(const glm::vec3& pos) = 0;
    void rasterize(core::rasterization::RasterizationRenderer* renderer, int pass) override {}

    bool intersect(const core::raytracing::Ray& ray, core::raytracing::HitRecord& record) const override { return false; }

    glm::vec3 calculateColor(core::raytracing::RaytracingRenderer* renderer, const core::raytracing::Ray& ray, float t) const override { return {}; }
private:
protected:
};

} // namespace components

#endif //SEELE_LIGHT_H
