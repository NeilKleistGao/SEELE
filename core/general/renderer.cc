/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file renderer.cc

#include "renderer.h"

#include <iostream>

namespace core::general {

Renderer::Renderer(const std::string& script_name, std::string output, int width, int height)
    : _process(0.0f), _image(new Image{std::move(output), width, height}){
    _state = luaL_newstate();
    luaL_openlibs(_state);

    auto res = luaL_dofile(_state, script_name.c_str());
    if (res) {
        std::cerr << "can't open file " << script_name << std::endl;
        exit(-1);
    }
}

Renderer::~Renderer() {
    delete _image;
    _image = nullptr;
}

void Renderer::registerComponents() {
    // TODO:
}

void Renderer::create() {
    auto list = luabridge::getGlobal(_state, "SHADERS_LIST");
    if (list.isTable()) {
        int size = list.length();
        for (int i = 1; i < size; ++i) {
            auto item = list[i];
            auto name = item[1].cast<std::string>();
            auto func = item[2];
            _shaders[name] = func;
        }
    }

    auto function = luabridge::getGlobal(_state, "onCreate");
    if (!function.isFunction()) {
        return;
    }

    function();
}

Shader* Renderer::getShader(const std::string& name) {
    if (_shaders.find(name) == _shaders.end()) {
        return nullptr;
    }

    return _shaders[name];
}

} // namespace core::general
