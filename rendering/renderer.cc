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

#include <cmath>
#include <algorithm>

#include "window_manager/window_manager.h"
#include "camera.h"
#include "light/light_manager.h"

namespace rendering {
Renderer* Renderer::_instance = nullptr;

Renderer::Renderer() : _r{255}, _g(255), _b(255), _a(255),
                        _enable_back_face_culling(CullingFace::NONE),
                        _z_buffer(nullptr), _texture(nullptr), _freeze(false), _normal(nullptr) {
}

Renderer* Renderer::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) Renderer();
        if (_instance == nullptr) {
            Debug::terminate("can't create renderer instance.");
        }
    }

    auto win = window_manager::Window::getInstance();
    _instance->_width = win->_width; _instance->_height = win->_height;

    auto size = _instance->_width * _instance->_height;
    _instance->_z_buffer = new float[size];
    for (int i = 0; i < size; ++i) {
        _instance->_z_buffer[i] = -1e9;
    }

    return _instance;
}

void Renderer::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void Renderer::clearWithColor(const unsigned char& r, const unsigned char& g, const unsigned char& b) {
    auto window = window_manager::Window::getInstance();
    auto size = _width * _height;
    for (int i = 0; i < size; i++) {
        window->_buffer2[(i << 2)] = r;
        window->_buffer2[(i << 2) | 1] = g;
        window->_buffer2[(i << 2) | 2] = b;
        window->_buffer2[(i << 2) | 3] = 255;
        _z_buffer[i] = -1e9;
    }
}

void Renderer::line(const math::Vector& begin, const math::Vector& end) {
    int dx = std::round(std::fabs(end.x - begin.x)),
    dy = std::round(std::fabs(end.y - begin.y));
    int fx = (end.x > begin.x) ? 1 : -1, fy = (end.y > begin.y) ? 1 : -1;

    if (dx == 0 && dy == 0) {
        setPixel(begin.x, begin.y, begin.z, getColor());
        return;
    }
    else if (dx == 0) {
        for (int i = static_cast<int>(begin.y); i != static_cast<int>(end.y); i += fy) {
            float t = (i - begin.y) / (end.y - begin.y);
            setPixel(begin.x, i, (1 - t) * begin.z + t * end.z, getColor());
        }
    }
    else if (dy == 0) {
        for (int i = static_cast<int>(begin.x); i != static_cast<int>(end.x); i += fx) {
            float t = (i - begin.x) / (end.x - begin.x);
            setPixel(i, begin.y, (1 - t) * begin.z + t * end.z, getColor());
        }
    }
    else if (dx == dy) {
        for (int i = static_cast<int>(begin.x), j = static_cast<int>(begin.y);
            i != static_cast<int>(end.x); i += fx, j += fy) {
            float t = (i - begin.x) / (end.x - begin.x);
            setPixel(i, j, (1 - t) * begin.z + t * end.z, getColor());
        }
    }
    else if (dx > dy) {
        int p = 2 * dy - dx, ddy = 2 * dy, ds = 2 * (dy - dx);
        setPixel(begin.x, begin.y, begin.z, getColor());
        for (int i = static_cast<int>(begin.x) + 1, j = static_cast<int>(begin.y); i != static_cast<int>(end.x); i += fx) {
            float t = (i - begin.x) / (end.x - begin.x);
            if (p < 0) {
                setPixel(i, j, (1 - t) * begin.z + t * end.z, getColor());
                p += ddy;
            }
            else {
                j += fy;
                setPixel(i, j, (1 - t) * begin.z + t * end.z, getColor());
                p += ds;
            }
        }
    }
    else {
        int p = 2 * dx - dy, ddx = 2 * dx, ds = 2 * (dx - dy);
        setPixel(begin.x, begin.y, begin.z, getColor());
        for (int i = static_cast<int>(begin.y) + 1, j = static_cast<int>(begin.x); i != static_cast<int>(end.y); i += fy) {
            float t = (i - begin.y) / (end.y - begin.y);
            if (p > 0) {
                setPixel(j, i, (1 - t) * begin.z + t * end.z, getColor());
                p += ddx;
            }
            else {
                j += fx;
                setPixel(j, i, (1 - t) * begin.z + t * end.z, getColor());
                p += ds;
            }
        }
    }

    setPixel(end.x, end.y, end.z, getColor());
}

void Renderer::setPixel(const int& x, const int& y, const float& z, const math::Vector& pixel) {
    auto window = window_manager::Window::getInstance();
    auto camera = Camera::getInstance();
    if (x >= 0 && y >= 0 && x < window->_width && y < window->_height &&
        _z_buffer[y * _width + x] <= z && z <= camera->getZNear() && z >= camera->getZFar()) {
        window->_buffer2[((_width * y + x) << 2) | 0] = pixel.x;
        window->_buffer2[((_width * y + x) << 2) | 1] = pixel.y;
        window->_buffer2[((_width * y + x) << 2) | 2] = pixel.z;
        window->_buffer2[((_width * y + x) << 2) | 3] = pixel.w;
        _z_buffer[y * _width + x] = z;
    }
}

void Renderer::triangle(math::Vector v1, math::Vector v2, math::Vector v3,
                        math::Vector tv1, math::Vector tv2, math::Vector tv3,
                        math::Vector nv1, math::Vector nv2, math::Vector nv3) {
//    if (cullBackFace(v1, v2, v3)) {
//        return;
//    }

    auto rs = script::RenderingScript::getInstance();
    rs->call("onVertex", v1, tv1);
    v1 = rs->getGlobalVariable<math::Vector>("out_position"); tv1 = rs->getGlobalVariable<math::Vector>("out_tex_coord");
    rs->call("onVertex", v2, tv2);
    v2 = rs->getGlobalVariable<math::Vector>("out_position"); tv2 = rs->getGlobalVariable<math::Vector>("out_tex_coord");
    rs->call("onVertex", v3, tv3);
    v3 = rs->getGlobalVariable<math::Vector>("out_position"); tv3 = rs->getGlobalVariable<math::Vector>("out_tex_coord");

    int min_x = std::min(std::min(v1.x, v2.x), v3.x), max_x = std::max(std::max(v1.x, v2.x), v3.x),
        min_y = std::min(std::min(v1.y, v2.y), v3.y), max_y = std::max(std::max(v1.y, v2.y), v3.y);

    for (int i = min_x; i <= max_x; ++i) {
        for (int j = min_y; j <= max_y; ++j) {
            auto temp = math::Vector{i, j, 0, 0};
            auto e1 = v1 - temp, e2 = v2 - temp, e3 = v3 - temp;

            auto r1 = e1.x * e2.y - e1.y * e2.x,
                 r2 = e2.x * e3.y - e2.y * e3.x,
                 r3 = e3.x * e1.y - e3.y * e1.x;
            if ((r1 >= -EPSILON && r2 >= -EPSILON && r3 >= -EPSILON) || (r1 <= EPSILON && r2 <= EPSILON && r3 <= EPSILON)) {
                float u = (-(i - v2.x) * (v3.y - v2.y) + (j - v2.y) * (v3.x - v2.x))
                          / (-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x)),
                        v = (-(i - v3.x) * (v1.y - v3.y) + (j - v3.y) * (v1.x - v3.x))
                            / (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x)),
                        w = 1 - u - v;
                auto normal = (nv1 * u + nv2 * v + nv3 * w).normalize();
                auto coord = tv1 * u + tv2 * v + tv3 * w;
                rasterize(math::Vector{i, j, v1.z * w + v2.z * u + v3.z * v},
                          coord, normal);
            }
        }
    }
}

void Renderer::flush() {
    auto window = window_manager::Window::getInstance();
    window->display();
}

bool Renderer::cullBackFace(math::Vector v1, math::Vector v2, math::Vector v3) {
    if (_enable_back_face_culling == CullingFace::NONE) {
        return false;
    }

//    auto e1 = v1 - v2, e2 = v2 - v3;
//    auto norm = e1 ^ e2;
//    auto dot = norm * Camera::getInstance()->getPosition();
//    return (dot >= 0 && _enable_back_face_culling == CullingFace::BACK_FACE) ||
//    (dot <= 0 && _enable_back_face_culling == CullingFace::FRONT_FACE);
    // TODO:
}

void Renderer::rasterize(const math::Vector& v, const math::Vector& tv, const math::Vector& n) {
    auto rs = script::RenderingScript::getInstance();
    auto lm = light::LightManager::getInstance();
    rs->call("onFragment", tv);
    auto color = rs->getGlobalVariable<math::Vector>("out_pixel");
    color = lm->calculateLight(v, n, color, 0.8f, 1.2f, 0.1f);
    setPixel(static_cast<int>(std::round(v.x)), static_cast<int>(std::round(v.y)), v.z, color);
}

math::Vector Renderer::getTexturePixel(const float& x, const float& y) {
    if (_texture == nullptr) {
        return math::Vector{};
    }

    auto width = _texture->get_width(), height = _texture->get_height();
    auto color = _texture->get(static_cast<int>(std::round(x * width)), static_cast<int>(std::round(y * height)));
    return math::Vector{color.r, color.g, color.b, color.a};
}

math::Vector Renderer::getNormalTextureDirection(const float& x, const float& y) {
    if (_normal == nullptr) {
        return math::Vector{};
    }

    auto width = _normal->get_width(), height = _normal->get_height();
    auto color = _normal->get(static_cast<int>(std::round(x * width)), static_cast<int>(std::round(y * height)));
    return math::Vector{color.r, color.g, color.b, color.a};
}

} // namespace rendering