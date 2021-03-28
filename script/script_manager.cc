/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file script_manager.cc

#include "script_manager.h"

#include "utils/debug.h"

namespace script {
ScriptManager* ScriptManager::_instance = nullptr;

ScriptManager* ScriptManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) ScriptManager();
        if (_instance == nullptr) {
            utils::Debug::terminate("can't create script manager!");
        }
    }

    return _instance;
}

void ScriptManager::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

ScriptManager::ScriptManager() : _state(nullptr) {
    _state = luaL_newstate();
    luaL_openlibs(_state);
}

ScriptManager::~ScriptManager() {
    if (_state != nullptr) {
        lua_close(_state);
        _state = nullptr;
    }
}

} // namespace script