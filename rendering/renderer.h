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

#include <list>
#include <array>

#include "utils/debug.h"
#include "utils/registration.h"
#include "script/rendering_script.h"
#include "math/vector.h"
#include "tga/tgaimage.h"

namespace rendering {

class Renderer {
public:
    static Renderer* getInstance();
    static void destroyInstance();

    void clearWithColor(const unsigned char& r, const unsigned char& g, const unsigned char& b);

    inline void setColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a = 255) {
        _r = r; _g = g; _b = b; _a = a;
    }

    void line(const math::Vector& begin, const math::Vector& end);

    void triangle(math::Vector v1, math::Vector v2, math::Vector v3);

    void triangleWithTexture(math::Vector v1, math::Vector v2, math::Vector v3,
                             const math::Vector& tv1, const math::Vector& tv2, const math::Vector& tv3);

    inline size_t getWidth() const {
        return _width;
    }

    inline size_t getHeight() const {
        return _height;
    }

    void flush();

    enum class CullingFace {
        NONE = 0,
        FRONT_FACE = 1,
        BACK_FACE = 2
    };

    inline void setCullingFace(const int& face) {
        _enable_back_face_culling = static_cast<CullingFace>(face);
    }

    inline void bindTexture(TGAImage* texture) {
        _texture = texture;
    }

private:
    using Debug = utils::Debug;

    Renderer();
    ~Renderer() = default;

    void setPixel(const int& x, const int& y, const int& z = 0);

    bool cullBackFace(math::Vector v1, math::Vector v2, math::Vector v3);

    static Renderer* _instance;

    unsigned char _r, _g, _b, _a;

    size_t _width, _height;

    CullingFace _enable_back_face_culling;

    int* _z_buffer;

    TGAImage* _texture;

    static constexpr float EPSILON = 1e-8;
};

SEELE_REGISTRATION(Renderer) {
    luabridge::getGlobalNamespace(script::RenderingScript::getInstance()->getState())
        .beginNamespace("seele")
            .beginClass<Renderer>("Renderer")
                .addStaticFunction("getInstance", &Renderer::getInstance)
                .addFunction("clearWithColor", &Renderer::clearWithColor)
                .addFunction("setColor", &Renderer::setColor)
                .addFunction("line", &Renderer::line)
                .addFunction("triangle", &Renderer::triangle)
                .addFunction("flush", &Renderer::flush)
                .addFunction("setCullingFace", &Renderer::setCullingFace)
            .endClass()
        .endNamespace();
}

} // namespace rendering

#endif //SEELE_RENDERER_H
