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

namespace script {

class Shader {
public:
    Shader(const luabridge::LuaRef& function, lua_State*& state) : _function(function), _state(state) {}
    ~Shader() = default;

    std::vector<glm::vec4> onVertex(const glm::vec4& app_data);
    glm::vec3 onFragment(const std::vector<glm::vec4>& v2f);
private:
    lua_State*& _state;
    luabridge::LuaRef _function;
};

} // namespace script

#endif //SEELE_SHADER_H
