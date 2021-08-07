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
#include "components/directional_light.h"
#include "../rasterization/rasterization_renderer.h"
#include "components/sphere.h"
#include "utilities/random.h"

namespace core::general {

Renderer::Renderer(const std::string& script_name, std::string output, int width, int height)
    : _process(0.0f), _image(new Image{std::move(output), width, height}),
    _current(nullptr), _state(nullptr), _width(width), _height(height), _camera(nullptr) {
    _state = luaL_newstate();
    luaL_openlibs(_state);

    _textures[0] = _textures[1] = _textures[2] = _textures[3] = nullptr;
    _textures[4] = _textures[5] = _textures[6] = _textures[7] = nullptr;

    auto res = luaL_dofile(_state, script_name.c_str());
    if (res) {
        std::cerr << "can't open file " << script_name << std::endl;
        exit(-1);
    }

    registerComponents();
    luabridge::setGlobal(_state, this, "R");
}

Renderer::~Renderer() {
    delete _image;
    _image = nullptr;
}

void Renderer::registerComponents() {
    using namespace glm;
    using namespace components;
    using namespace assets;

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

        .beginClass<Material>("Material")
            .addProperty("Kd", &Material::Kd, false)
            .addProperty("Ka", &Material::Ka, false)
            .addProperty("Ks", &Material::Ks, false)
        .endClass()

        .beginClass<Renderer>("Renderer")
            .addFunction("addObject", &Renderer::addObject)
            .addFunction("setCamera", &Renderer::setCamera)
            .addFunction("transformMVP", &Renderer::transformMVP)
            .addFunction("getTextureColor", &Renderer::getTextureColor)
            .addFunction("transformNormal", &Renderer::transformNormal)
            .addFunction("getMaterial", &Renderer::getMaterial)
        .endClass()

        .deriveClass<rasterization::RasterizationRenderer, Renderer>("RasterizationRenderer")
            .addFunction("setPass", &rasterization::RasterizationRenderer::setPass)
        .endClass()

        .beginClass<Transform>("Transform")
            .addFunction("setPosition", &Transform::setPosition)
            .addFunction("setRotation", &Transform::setRotation)
            .addFunction("setScale", &Transform::setScale)
            .addFunction("getPosition", &Transform::getPosition)
            .addFunction("getRotation", &Transform::getRotation)
            .addFunction("getScale", &Transform::getScale)
        .endClass()

        .beginClass<Light::LightData>("LightData")
            .addProperty("direction", &Light::LightData::direction)
            .addProperty("color", &Light::LightData::color)
        .endClass()

        .deriveClass<Camera, Transform>("Camera")
            .addConstructor<void (*) (const glm::vec3&, const glm::vec3&, const glm::vec3&, float, float, float)>()
        .endClass()

        .deriveClass<TriMesh, Transform>("TriMesh")
            .addConstructor<void (*) (std::string, const luabridge::LuaRef&, const luabridge::LuaRef&)>()
        .endClass()

        .deriveClass<Sphere, Transform>("Sphere")
            .addConstructor<void (*) (const float&, const luabridge::LuaRef&, const luabridge::LuaRef&)>()
        .endClass()

        .deriveClass<Light, Transform>("Light")
            .addFunction("getLightData", &Light::getLightData)
        .endClass()

        .deriveClass<DirectionalLight, Light>("DirectionalLight")
            .addConstructor<void (*) (const glm::vec3&, const glm::vec3&)>()
        .endClass()

        .addFunction<glm::vec3, const glm::vec3&>("normalize", &glm::normalize)

        .beginClass<raytracing::HitRecord>("HitRecord")
            .addProperty("position", &raytracing::HitRecord::position)
            .addProperty("normal", &raytracing::HitRecord::normal)
            .addProperty("time", &raytracing::HitRecord::time)
            .addProperty("front", &raytracing::HitRecord::front)
        .endClass()

        .beginClass<raytracing::Ray>("Ray")
            .addConstructor<void (*) (const glm::vec3&, const glm::vec3&)>()
            .addFunction("getDirection", &raytracing::Ray::getDirection)
        .endClass()

        .beginClass<utilities::Random>("Random")
            .addConstructor<void (*) (void)>()
            .addFunction<float, float>("roll", &utilities::Random::roll)
        .endClass()
    .endNamespace();
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

glm::vec3 Renderer::getTextureColor(int i, float x, float y) {
    if (_textures[i] == nullptr) {
        return {};
    }

    return _textures[i]->getColor(x, y);
}

glm::vec3 Renderer::transformNormal(const glm::vec3& n) {
    auto n4 = glm::vec4{n.x, n.y, n.z, 0};
    if (_current != nullptr) {
        n4 = _current->transformNormal(n4);
    }

    if (_camera != nullptr) {
        n4 = _camera->transformNormal(n4);
    }

    return glm::normalize(glm::vec3 {n4.x, n4.y, n4.z});
}

void Renderer::loadTexture(int i, const std::string& filename) {

}
} // namespace core::general
