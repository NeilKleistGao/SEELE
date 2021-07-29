/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file shader.cc

#include "shader.h"

namespace core::general {

Shader::Shader(lua_State* state, const std::string& function_name) : _function(state), _state(state) {
    _function = luabridge::getGlobal(state, function_name.c_str());
    if (!_function.isFunction()) {
        _function = luabridge::LuaRef{state};
    }
}

std::vector<Shader::ShaderDataItem> Shader::onVertex(const std::vector<ShaderDataItem>& appdata) {
    if (_function.isFunction()) {
        auto table = createTable(appdata);
        auto temp = _function(table);
        std::vector<ShaderDataItem> res = parseTable(temp);
        return res;
    }
    else {
        return std::vector<ShaderDataItem>{};
    }
}

glm::vec3 Shader::onFragment(const std::vector<ShaderDataItem>& v2f) {
    if (!_function.isFunction()) {
        return glm::vec3 {};
    }

    auto table = createTable(v2f);
    return _function(table).cast<glm::vec3>();
}

luabridge::LuaRef Shader::createTable(const std::vector<ShaderDataItem>& data) {
    auto table = luabridge::newTable(_state);
    for (const auto& item : data) {
        switch (item.dimension) {
            case 1:
                table.append(item.f);
                break;
            case 2:
                table.append(item.vec2);
                break;
            case 3:
                table.append(item.vec3);
                break;
            case 4:
                table.append(item.vec4);
                break;
        }
    }

    return table;
}

std::vector<Shader::ShaderDataItem> Shader::parseTable(const luabridge::LuaRef& table) {
    std::vector<ShaderDataItem> res;
    if (table.isTable()) {
        for (int i = 1; i <= table.length(); ++i) {
            auto item = table[i];
            ShaderDataItem v2f{};
            v2f.dimension = item[DIMENSION_INDEX].cast<int>();
            v2f.interpolation = item[INTERPOLATION_INDEX].cast<bool>();
            switch (v2f.dimension) {
                case 1:
                    v2f.f = item[DATA_INDEX].cast<float>();
                    break;
                case 2:
                    v2f.vec2 = item[DATA_INDEX].cast<glm::vec2>();
                    break;
                case 3:
                    v2f.vec3 = item[DATA_INDEX].cast<glm::vec3>();
                    break;
                case 4:
                    v2f.vec4 = item[DATA_INDEX].cast<glm::vec4>();
                    break;
                default:
                    continue;
            }

            res.push_back(v2f);
        }
    }

    return res;
}

Shader::ShaderDataItem::ShaderDataItem(const ShaderDataItem& other)
    : interpolation(other.interpolation), dimension(other.dimension) {
    switch (dimension) {
        case 1:
            f = other.f;
            break;
        case 2:
            vec2 = other.vec2;
            break;
        case 3:
            vec3 = other.vec3;
            break;
        case 4:
            vec4 = other.vec4;
            break;
    }
}

bool Shader::scatter(const core::raytracing::Ray& ray, const core::raytracing::HitRecord& record,
             glm::vec3& color, core::raytracing::Ray& scattered) {
    auto res_table = _function(ray, record);
    auto bool_res = res_table[1];
    auto color_res = res_table[2];
    auto scattered_res = res_table[3];

    color = luabridge::LuaRef_cast<glm::vec3>(color_res);
    scattered = luabridge::LuaRef_cast<core::raytracing::Ray>(scattered_res);
    return luabridge::LuaRef_cast<bool>(bool_res);
}

} // namespace core::general