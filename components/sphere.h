/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file sphere.h

#ifndef SEELE_SPHERE_H
#define SEELE_SPHERE_H

#include <string>

#include "transform.h"
#include "core/general/shader.h"
#include "assets/texture.h"

namespace components {

class Sphere : public Transform {
public:
    Sphere(float radius, const luabridge::LuaRef& vertex_shader, const luabridge::LuaRef& fragment_shader);
    ~Sphere() override = default;

    void rasterize(core::rasterization::RasterizationRenderer* renderer, int pass) override;

    float intersect(const core::raytracing::Ray& ray) const override;
    glm::vec3 calculateColor(core::raytracing::RaytracingRenderer* renderer, const core::raytracing::Ray& ray, float t) const override;
private:
    using ShaderDataItem = core::general::Shader::ShaderDataItem;
    using ShaderDataList = std::vector<ShaderDataItem>;

    constexpr static float PI = 3.1415926535f;

    float _radius;

    luabridge::LuaRef _vertex_shader;
    luabridge::LuaRef _fragment_shader;

    glm::vec2 getUV(const glm::vec3& pos) const;

    inline glm::vec3 getNormal(const glm::vec3& pos) const {
        return glm::normalize(pos);
    }
protected:
};

} // namespace components

#endif //SEELE_SPHERE_H
