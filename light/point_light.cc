/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file point_light.cc

#include "point_light.h"

#include "rendering/camera.h"

namespace light {

PointLight::PointLight(math::Vector position, float intensity)
: _position(position), _intensity(intensity), _color(255, 255, 255, 255) {

}

math::Vector PointLight::calculateLight(const math::Vector& position,
                            const math::Vector& normal,
                            const math::Vector& color,
                            const float& ambient,
                            const float& diffuse,
                            const float& specular) {
    math::Vector output{0, 0, 0, 0};
    float attenuation = 125 / _position.getDistance(position);
    output += _intensity * ambient * attenuation * color;

    auto direction = _position - position;
    auto cth = direction.normalize() * normal.normalize();
    if (cth > 0) {
        output += _intensity * diffuse * cth * attenuation * color;
    }

    auto camera = rendering::Camera::getInstance();
    auto cp = camera->getPosition(), cs = camera->getCameraSize();
    auto direction2 = 2 * (normal * direction) * normal;
    auto direction3 = math::Vector{cp.x / cs.x * 1024, cp.y / cs.y * 768, cp.z + 10} - position;
    if (direction3 * direction2 > 0 && normal * direction > 0) {
        auto cph = direction2.normalize() * direction3.normalize();
        output += _intensity * specular * cph * attenuation * color;
    }

    return math::Vector{output.x * _color.x / 255, output.y  * _color.y / 255,
                        output.z  * _color.z / 255, output.w  * _color.w / 255};
}

} // namespace light