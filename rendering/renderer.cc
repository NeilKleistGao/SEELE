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
#include "rasterization_renderer.h"

#include <algorithm>

namespace rendering {
Renderer* Renderer::_instance = nullptr;

Renderer::Renderer() : _buffer(nullptr) {}

Renderer::~Renderer() {
    _buffer->flush();
    delete _buffer;
    _buffer = nullptr;
}

Renderer* Renderer::getInstance(RenderingMethod method) {
    if (_instance == nullptr) {
        switch (method) {
            case RenderingMethod::RENDERING_RASTERIZATION:
                _instance = new(std::nothrow) RasterizationRenderer();
                break;
            case RenderingMethod::RENDERING_RAY_TRACING:
                break;
            default:
                break;
        }
    }

    return _instance;
}

void Renderer::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void Renderer::init(const std::string& filename, const size_t& width, const size_t& height) {
    _width = width; _height = height;
    _buffer = new Image{filename, width, height};
}

} // namespace rendering