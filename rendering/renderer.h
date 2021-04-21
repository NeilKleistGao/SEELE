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

#include <vector>
#include <map>

#include "i_element.h"
#include "image.h"
#include "script/shader.h"

namespace rendering {

enum class RenderingMethod {
    RENDERING_INHERIT = -1,
    RENDERING_RASTERIZATION = 0,
    RENDERING_RAY_TRACING
};

class Renderer {
public:
    static Renderer* getInstance(RenderingMethod method = RenderingMethod::RENDERING_INHERIT);
    static void destroyInstance();

    virtual void render() = 0;

    void init(const std::string& filename, const size_t& width, const size_t& height);

    inline void addElement(IElement* element) {
        _elements.push_back(element);
    }

    glm::vec4 transformMVP(const glm::vec4& vec);

    void addShader(const std::string& name, script::Shader* shader) {
        _shaders[name] = shader;
    }

    script::Shader* getShader(const std::string& name);
private:
    static Renderer* _instance;
protected:
    Renderer();
    virtual ~Renderer();

    size_t _width;
    size_t _height;

    std::vector<IElement*> _elements;
    std::map<std::string, script::Shader*> _shaders;
    Image* _buffer;

    IElement* _current;
};
} // namespace rendering

#endif //SEELE_RENDERER_H
