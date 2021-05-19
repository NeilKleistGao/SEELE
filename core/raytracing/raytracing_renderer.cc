/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file raytracing_renderer.cc

#include "raytracing_renderer.h"

#include <utility>

#include "ray.h"

namespace core::raytracing {

RaytracingRenderer::RaytracingRenderer(const std::string& script_name, std::string output, int width, int height)
    : general::Renderer(script_name, std::move(output), width, height) {

}

void RaytracingRenderer::render() {
    create();
    if (_camera == nullptr) {
        return;
    }

    auto origin = _camera->getOrigin();
    int total = _width * _height, finished = 0;
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            Ray ray(origin, _camera->getPixelPosition(i, j) - origin);
            float min = -1.0f;
            const components::Transform* hit = nullptr;

            for (const auto* trans : _transforms) {
                float res = trans->intersect(ray);
                if (!std::isnan(res) && res > 0) {
                    if (min < 0 || res < min) {
                        min = res;
                        hit = trans;
                    }
                }
            }

            if (hit != nullptr) {
                _image->putPixel(i, j, 0, 0, 255);
            }

            ++finished;
            _process = static_cast<float>(finished) / static_cast<float>(total);
        }
    }
}

} // namespace core::raytracing