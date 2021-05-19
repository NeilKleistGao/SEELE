/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file sphere.cc

#include "sphere.h"

#include <cmath>

#include "core/rasterization/rasterization_renderer.h"

namespace components {

Sphere::Sphere(float radius,
               const luabridge::LuaRef& vertex_shader,
               const luabridge::LuaRef& fragment_shader)
               : Transform(), _vertex_shader(vertex_shader),
               _fragment_shader(fragment_shader), _radius(radius) {
}

void Sphere::rasterize(core::rasterization::RasterizationRenderer* renderer, int pass) {
    updateModelMatrix();
    auto v_shader = renderer->getShader(_vertex_shader[pass]);
    auto f_shader = renderer->getShader(_fragment_shader[pass]);

    for (int i = -_radius; i <= _radius; ++i) {
        for (int j = -_radius; j <= _radius; j++) {
            for (int k = -_radius; k <= _radius; ++k) {
                float x = i, y = j, z = k;
                if (x * x + y * y + z * z <= _radius * _radius) {
                    ShaderDataList appdata;
                    appdata.emplace_back(); appdata.emplace_back(); appdata.emplace_back();
                    appdata[0].dimension = 3; appdata[0].vec3 = {x, y, z}; appdata[0].interpolation = false;
                    appdata[1].dimension = 2; appdata[1].vec2 = getUV({x, y, z}); appdata[1].interpolation = false;
                    appdata[2].dimension = 3; appdata[2].vec3 = getNormal({x, y, z}); appdata[2].interpolation = false;

                    auto v2f = v_shader->onVertex(appdata);
                    auto color = f_shader->onFragment(v2f);
                    renderer->putPixel({x, y, z}, color);
                }

                x += 0.5f; y += 0.5f; z += 0.5f;
                if (x * x + y * y + z * z <= _radius * _radius) {
                    ShaderDataList appdata;
                    appdata.emplace_back(); appdata.emplace_back(); appdata.emplace_back();
                    appdata[0].dimension = 3; appdata[0].vec3 = {x, y, z}; appdata[0].interpolation = false;
                    appdata[1].dimension = 2; appdata[1].vec2 = getUV({x, y, z}); appdata[1].interpolation = false;
                    appdata[2].dimension = 3; appdata[2].vec3 = getNormal({x, y, z}); appdata[2].interpolation = false;

                    auto v2f = v_shader->onVertex(appdata);
                    auto color = f_shader->onFragment(v2f);
                    renderer->putPixel({x, y, z}, color);
                }
            }
        }
    }
}

glm::vec2 Sphere::getUV(const glm::vec3& pos) const {
    float phi = std::atan2(pos.z, pos.x),
          theta = asin(pos.y / _radius);

    return glm::vec2(1 - (phi + PI) / (2 * PI), (theta + PI / 2) / PI);
}

float Sphere::intersect(const core::raytracing::Ray& ray) const {
    auto sub = ray.getOrigin() - _position;

    float a = glm::dot(ray.getDirection(), ray.getDirection()),
          b = 2.0f * glm::dot(ray.getDirection(), sub),
          c = glm::dot(sub, sub) - _radius * _radius;

    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        return NAN;
    }

    float t1 = (-b + std::sqrt(delta)) / (2 * a),
          t2 = (-b - std::sqrt(delta)) / (2 * a);

    if (t1 > t2) {
        std::swap(t1, t2);
    }

    return (t1 < 0) ? t2 : t1;
}

glm::vec3 Sphere::calculateColor(const core::raytracing::Ray& ray, float t) const {
    // TODO:
    return {};
}

} // namespace components