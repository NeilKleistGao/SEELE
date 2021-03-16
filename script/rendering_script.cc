/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file rendering_script.cc

#include "rendering_script.h"

namespace script {
RenderingScript* RenderingScript::_instance = nullptr;

RenderingScript* RenderingScript::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) RenderingScript();
        if (_instance == nullptr) {
            Debug::terminate("can't create rendering script!");
        }
    }

    return _instance;
}

void RenderingScript::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void RenderingScript::preload(const std::filesystem::path& path) {
    std::filesystem::directory_iterator dit{path};
    for (const auto& it : dit) {
        auto res = luaL_dofile(_state, it.path().c_str());
        if (res) {
            Debug::terminate("preloading api failed!");
        }
    }
}

void RenderingScript::load(const std::string& filename) {
    auto res = luaL_dofile(_state, filename.c_str());
    if (res) {
        Debug::terminate("preloading api failed!");
    }

    luabridge::LuaRef on_load = luabridge::getGlobal(_state, "onLoad");
    if (on_load.isFunction()) {
        on_load();
    }
}

void RenderingScript::update(const float& delta) {
    luabridge::LuaRef on_update = luabridge::getGlobal(_state, "onUpdate");
    if (on_update.isFunction()) {
        on_update(delta);
    }
}

RenderingScript::RenderingScript() : _state(nullptr) {
    _state = luaL_newstate();
    luaL_openlibs(_state);

}

RenderingScript::~RenderingScript() {
    if (_state != nullptr) {
        lua_close(_state);
        _state = nullptr;
    }
}

} // namespace script