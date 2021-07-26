/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file shader.h

#ifndef SEELE_SHADER_H
#define SEELE_SHADER_H

#include <vector>

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "glm/glm.hpp"
#include "core/raytracing/ray.h"
#include "core/raytracing/hit_record.h"

namespace core::general {

class Shader {
public:
    struct ShaderDataItem {
        union {
            float f;
            glm::vec2 vec2;
            glm::vec3 vec3;
            glm::vec4 vec4;
        };

        int dimension;
        bool interpolation;

        ShaderDataItem() = default;
        ShaderDataItem(const ShaderDataItem& other);
    };

    Shader(lua_State* state, const std::string& function_name);

    std::vector<ShaderDataItem> onVertex(const std::vector<ShaderDataItem>& appdata);
    glm::vec3 onFragment(const std::vector<ShaderDataItem>& v2f);

    bool scatter(const core::raytracing::Ray& ray, const core::raytracing::HitRecord& record,
                 glm::vec3& color, core::raytracing::Ray& scattered);
private:
    lua_State* _state;
    luabridge::LuaRef _function;

    static constexpr int DIMENSION_INDEX = 1;
    static constexpr int DATA_INDEX = 2;
    static constexpr int INTERPOLATION_INDEX = 3;

    luabridge::LuaRef createTable(const std::vector<ShaderDataItem>& data);
    std::vector<ShaderDataItem> parseTable(const luabridge::LuaRef& table);
protected:
};

}

#endif //SEELE_SHADER_H