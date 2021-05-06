/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file renderer.h

#ifndef SEELE_RENDERER_H
#define SEELE_RENDERER_H

#include <map>
#include <list>

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "image.h"
#include "shader.h"
#include "components/transform.h"
#include "components/camera.h"
#include "assets/texture.h"
#include "components/light.h"
#include "assets/material.h"

namespace core::general {

class Renderer {
public:
    Renderer(const std::string& script_name, std::string output, int width, int height);
    virtual ~Renderer();
    Renderer(const Renderer& other) = delete;
    Renderer& operator= (const Renderer& other) = delete;

    virtual void render() {};

    inline float getProcess() const {
        return _process;
    }

    inline void addObject(components::Transform* transform) {
        _transforms.push_back(transform);
    }

    Shader* getShader(const std::string& name);

    glm::vec3 transformMVP(const glm::vec3& v);

    inline void setCamera(components::Camera* camera) {
        _camera = camera;
    }

    inline void setTexture(int i, assets::Texture* texture) {
        _textures[i] = texture;
    }

    glm::vec3 getTextureColor(int i, float x, float y);

    glm::vec3 transformNormal(const glm::vec3& n);

    inline void setMaterial(assets::Material material) {
        _material = material;
    }

    inline assets::Material getMaterial() const {
        return _material;
    }
private:
    constexpr static int TEXTURE_COUNT = 8;

    lua_State* _state;
    std::map<std::string, Shader*> _shaders;
    assets::Texture* _textures[TEXTURE_COUNT];
    assets::Material _material;

    void registerComponents();
protected:
    float _process;
    Image* _image;
    components::Transform* _current;
    components::Camera* _camera;
    int _width;
    int _height;

    std::list<components::Transform*> _transforms;

    std::list<components::Light*>::iterator _light_it;

    void create();
};

} // namespace core::general

#endif //SEELE_RENDERER_H
