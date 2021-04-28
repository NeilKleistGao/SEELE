/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file rasterization_renderer.cc

#include "rasterization_renderer.h"

#include <utility>
#include <limits>

namespace core::rasterization {

RasterizationRenderer::RasterizationRenderer(const std::string& script_name, std::string output, int width, int height)
    : general::Renderer(script_name, std::move(output), width, height) {
    _z_buffer = new float[width * height];
    for (int i = 0; i < width * height; ++i) {
        _z_buffer[i] = std::numeric_limits<float>::lowest();
    }
}

RasterizationRenderer::~RasterizationRenderer() {
    delete [] _z_buffer;
    _z_buffer = nullptr;
}

void RasterizationRenderer::render() {
    create();

    int total = _transforms.size();
    if (total == 0) {
        return;
    }

    int finished = 0;
    for (auto* transform : _transforms) {
        _current = transform;
        _current->rasterize(this);
        ++finished;
        _process = static_cast<float>(finished) / static_cast<float>(total);
    }
}

void RasterizationRenderer::putPixel(const glm::vec3& pos, const glm::vec3& color) {
    int x = pos.x + _width / 2, y = pos.y + _height / 2;
    int index = y * _width + x;
    if (index < _width * _height && _z_buffer[index] <= pos.z) {
        _image->putPixel(x, y, color.r, color.g, color.b);
        _z_buffer[index] = pos.z;
    }
}

} // namespace core::rasterization