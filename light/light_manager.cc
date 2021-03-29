/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */


/// @file light_manager.cc

#include "light_manager.h"

#include <algorithm>

#include "utils/debug.h"

namespace light {
LightManager* LightManager::_instance = nullptr;

LightManager* LightManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) LightManager();
        if (_instance == nullptr) {
            utils::Debug::terminate("can't create light manager.");
        }
    }

    return _instance;
}

void LightManager::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

math::Vector LightManager::calculateLight(const math::Vector& position,
                                          const math::Vector& normal,
                                          const math::Vector& color,
                                          const float& ambient,
                                          const float& diffuse,
                                          const float& specular) {
    math::Vector output{0, 0, 0, 0};
    for (auto* const p : _lights) {
        output += p->calculateLight(position, normal, color, ambient, diffuse, specular);
        output.x = std::min(output.x, 255.0f);
        output.y = std::min(output.y, 255.0f);
        output.z = std::min(output.z, 255.0f);
        output.w = std::min(output.w, 255.0f);
    }

    return output;
}

LightManager::LightManager() = default;

LightManager::~LightManager() {
    for (auto* p : _lights) {
        delete p;
        p = nullptr;
    }

    _lights.clear();
}

} // namespace light