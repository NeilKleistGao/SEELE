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
#include "utilities/random.h"

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
    auto random = utilities::Random();

    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            auto color = glm::vec3 {0, 0, 0};
            for (int k = 0; k < MULTIPLE_SAMPLE_TIMES; ++k) {
                float dx = random.roll(-1, 1),
                      dy = random.roll(-1, 1);
                Ray ray(origin, _camera->getPixelPosition((i + dx) / _width, (j + dy) / _height) - origin);
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
                    auto temp = hit->calculateColor(this, ray, min);
                    if (temp.x < 0 || temp.y < 0 || temp.z < 0) {
                        color.x = -1;
                        break;
                    }

                    color += temp;
                }
                else {
                    // temporal global illumination
                    auto t = 0.5f * (static_cast<float>(_height - j + 1 + dy) / _height + 1.0f);
                    color += t * glm::vec3(255, 255, 255) + (1.0f - t) * glm::vec3(127, 178, 255);
                }

            }

            if (color.x >= 0 && color.y >= 0 && color.z >= 0) {
                _image->putPixel(i, _height - j - 1, color.x / MULTIPLE_SAMPLE_TIMES,
                                 color.y / MULTIPLE_SAMPLE_TIMES,
                                 color.z / MULTIPLE_SAMPLE_TIMES);
            }

            ++finished;
            _process = static_cast<float>(finished) / static_cast<float>(total);
        }
    }
}

} // namespace core::raytracing