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

namespace script {

std::vector<glm::vec4> Shader::onVertex(const glm::vec4& app_data) {
    auto res = _function(app_data);
    std::vector<glm::vec4> list;

    if (res.isTable()) {
        for (int i = 1; i < res.length(); ++i) {
            list.push_back(res[i].cast<glm::vec4>());
        }
    }
    else {
        list.push_back(res.cast<glm::vec4>());
    }

    return list;
}

glm::vec3 Shader::onFragment(const std::vector<glm::vec4>& v2f) {
    auto list = luabridge::newTable(_state);
    for (const auto& v : v2f) {
        list.append(v);
    }

    auto color = _function(list);
    return color.cast<glm::vec3>();
}

} // namespace script
