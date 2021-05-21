/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file raytracing_renderer.h

#ifndef SEELE_RAYTRACING_RENDERER_H
#define SEELE_RAYTRACING_RENDERER_H

#include "core/general/renderer.h"

namespace core::raytracing {

class RaytracingRenderer : public general::Renderer {
public:
    RaytracingRenderer(const std::string& script_name, std::string output, int width, int height);
    void render() override;
private:
    static constexpr int MULTIPLE_SAMPLE_TIMES = 100;
protected:
};

} // namespace core::raytracing

#endif //SEELE_RAYTRACING_RENDERER_H
