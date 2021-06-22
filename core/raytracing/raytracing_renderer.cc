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

    int total = _width * _height, finished = 0;
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            auto color = traceColor(i, j);
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

glm::vec3 RaytracingRenderer::traceColor(int x, int y) {
    auto origin = _camera->getOrigin();
    auto random = utilities::Random();
    auto color = glm::vec3 {0, 0, 0};

    if (x == 200 && y == 170) {
        int a = 0;
    }

    for (int i = 0; i < MULTIPLE_SAMPLE_TIMES; ++i) {
        float dx = random.roll(-1, 1),
                dy = random.roll(-1, 1);
        Ray ray(origin, _camera->getPixelPosition((x + dx) / _width, (y + dy) / _height) - origin);
        auto temp = transport(ray, MAX_RECURSE_TIMES);
        if (temp.x >= 0 && temp.y >= 0 && temp.z >= 0) {
            color += temp;
        }
    }

    return color;
}

glm::vec3 RaytracingRenderer::transport(const Ray& ray, int depth) {
    if (depth == 0) {
        return glm::vec3 {0, 0, 0};
    }

    float min = -1.0f;
    const components::Transform* hit = nullptr;
    HitRecord record{}, temp{};

    for (const auto* trans : _transforms) {
        if (trans->intersect(ray, temp)) {
            if (min < 0 || temp.time < min) {
                min = temp.time;
                hit = trans;
                record = temp;
            }
        }
    }

    if (hit != nullptr) {
        Ray another{record.position, record.normal + getRandomDirectionInUnitSphere()};
        return 0.5f * transport(another, depth - 1);
    }
    else {
        // temporal global illumination
        auto t = 0.5f * (1.0f + ray.getDirection().z);
        return (1.0f - t) * glm::vec3(255, 255, 255) + t * glm::vec3(127, 178, 255);
    }
}

glm::vec3 RaytracingRenderer::getRandomDirectionInUnitSphere() {
    auto random = utilities::Random();
    while (true) {
        auto dir = glm::vec3 {random.roll(-1, 1), random.roll(-1, 1), random.roll(-1, 1)};
        if (glm::dot(dir, dir) >= 1) {
            continue;
        }

        return dir;
    }
}

} // namespace core::raytracing