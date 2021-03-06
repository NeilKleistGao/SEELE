/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file rasterization_renderer.h

#ifndef SEELE_RASTERIZATION_RENDERER_H
#define SEELE_RASTERIZATION_RENDERER_H

#include "core/general/renderer.h"

namespace core::rasterization {

class RasterizationRenderer : public general::Renderer {
public:
    RasterizationRenderer(const std::string& script_name, std::string output, int width, int height);
    ~RasterizationRenderer() override;
    void render() override;

    void putPixel(const glm::vec3& pos, const glm::vec3& color);

    inline void setPass(int pass) {
        _pass = pass;
    }
private:
    float* _z_buffer;

    int _pass;
protected:
};

} // namespace core::rasterization

#endif //SEELE_RASTERIZATION_RENDERER_H
