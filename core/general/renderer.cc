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

#include "glm/glm.hpp"
#include "components/transform.h"
#include "components/tri_mesh.h"
#include "components/camera.h"

namespace core::general {

Renderer::Renderer(const std::string& script_name, std::string output, int width, int height)
    : _process(0.0f), _image(new Image{std::move(output), width, height}), _current(nullptr), _state(nullptr) {
    _state = luaL_newstate();
    luaL_openlibs(_state);

    auto res = luaL_dofile(_state, script_name.c_str());
    if (res) {
        std::cerr << "can't open file " << script_name << std::endl;
        exit(-1);
    }

    registerComponents();
}

Renderer::~Renderer() {
    delete _image;
    _image = nullptr;
}

void Renderer::registerComponents() {
    using namespace glm;
    using namespace components;

    luabridge::getGlobalNamespace(_state).beginNamespace("seele")
        .beginClass<vec2>("vec2")
            .addConstructor<void (*) (float, float)>()
            .addProperty("x", &vec2::x)
            .addProperty("y", &vec2::y)
        .endClass()

        .beginClass<vec3>("vec3")
            .addConstructor<void (*) (float, float, float)>()
            .addProperty("x", &vec3::x)
            .addProperty("y", &vec3::y)
            .addProperty("z", &vec3::z)
        .endClass()

        .beginClass<vec4>("vec4")
            .addConstructor<void (*) (float, float, float, float)>()
            .addProperty("x", &vec4::x)
            .addProperty("y", &vec4::y)
            .addProperty("z", &vec4::z)
            .addProperty("w", &vec4::w)
        .endClass()

        .beginClass<Renderer>("Renderer")
            .addFunction("addObject", &Renderer::addObject)
            .addFunction("setCamera", &Renderer::setCamera)
            .addFunction("transformMVP", &Renderer::transformMVP)
        .endClass()

        .beginClass<Transform>("Transform")
            .addFunction("setPosition", &Transform::setPosition)
            .addFunction("setRotation", &Transform::setRotation)
            .addFunction("setScale", &Transform::setScale)
            .addFunction("getPosition", &Transform::getPosition)
            .addFunction("getRotation", &Transform::getRotation)
            .addFunction("getScale", &Transform::getScale)
        .endClass()

        .deriveClass<Camera, Transform>("Camera")
            .addConstructor<void (*) (const glm::vec3&, const glm::vec3&, const glm::vec3&, float, float, float)>()
        .endClass()

        .deriveClass<TriMesh, Transform>("TriMesh")
            .addConstructor<void (*) (std::string, std::string, std::string)>()
        .endClass()
    .endNamespace();

    luabridge::setGlobal(_state, this, "R");
}

void Renderer::create() {
    auto list = luabridge::getGlobal(_state, "SHADERS_LIST");
    if (list.isTable()) {
        int size = list.length();
        for (int i = 1; i <= size; ++i) {
            auto name = list[i].cast<std::string>();
            _shaders[name] = new Shader(_state, name);
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

glm::vec3 Renderer::transformMVP(const glm::vec3& v) {
    auto v4 = glm::vec4{v.x, v.y, v.z, 1};
    if (_current != nullptr) {
        v4 = _current->transform(v4);
    }

    if (_camera != nullptr) {
        v4 = _camera->transform(v4);
    }

    return glm::vec3 {v4.x, v4.y, v4.z};
}

} // namespace core::general
