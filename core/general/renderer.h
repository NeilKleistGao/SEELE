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

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "image.h"
#include "shader.h"

namespace core::general {

class Renderer {
public:
    Renderer(const std::string& script_name, std::string output, int width, int height);
    virtual ~Renderer();

    virtual void render() = 0;

    inline float getProcess() const {
        return _process;
    }
private:
    lua_State* _state;
    std::map<std::string, Shader*> _shaders;

    void registerComponents();
protected:
    float _process;
    Image* _image;

    void create();
};

} // namespace core::general

#endif //SEELE_RENDERER_H
