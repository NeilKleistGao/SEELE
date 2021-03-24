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

namespace rendering {
Renderer* Renderer::_instance = nullptr;

Renderer::Renderer() : _r{255}, _g(255), _b(255), _a(255),
                        _enable_back_face_culling(CullingFace::NONE),
                        _z_buffer(nullptr), _texture(nullptr), _freeze(false) {
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
        setPixel(begin.x, begin.y, begin.z);
        return;
    }
    else if (dx == 0) {
        for (int i = static_cast<int>(begin.y); i != static_cast<int>(end.y); i += fy) {
            float t = (i - begin.y) / (end.y - begin.y);
            setPixel(begin.x, i, (1 - t) * begin.z + t * end.z);
        }
    }
    else if (dy == 0) {
        for (int i = static_cast<int>(begin.x); i != static_cast<int>(end.x); i += fx) {
            float t = (i - begin.x) / (end.x - begin.x);
            setPixel(i, begin.y, (1 - t) * begin.z + t * end.z);
        }
    }
    else if (dx == dy) {
        for (int i = static_cast<int>(begin.x), j = static_cast<int>(begin.y);
            i != static_cast<int>(end.x); i += fx, j += fy) {
            float t = (i - begin.x) / (end.x - begin.x);
            setPixel(i, j, (1 - t) * begin.z + t * end.z);
        }
    }
    else if (dx > dy) {
        int p = 2 * dy - dx, ddy = 2 * dy, ds = 2 * (dy - dx);
        setPixel(begin.x, begin.y, begin.z);
        for (int i = static_cast<int>(begin.x) + 1, j = static_cast<int>(begin.y); i != static_cast<int>(end.x); i += fx) {
            float t = (i - begin.x) / (end.x - begin.x);
            if (p < 0) {
                setPixel(i, j, (1 - t) * begin.z + t * end.z);
                p += ddy;
            }
            else {
                j += fy;
                setPixel(i, j, (1 - t) * begin.z + t * end.z);
                p += ds;
            }
        }
    }
    else {
        int p = 2 * dx - dy, ddx = 2 * dx, ds = 2 * (dx - dy);
        setPixel(begin.x, begin.y, begin.z);
        for (int i = static_cast<int>(begin.y) + 1, j = static_cast<int>(begin.x); i != static_cast<int>(end.y); i += fy) {
            float t = (i - begin.y) / (end.y - begin.y);
            if (p > 0) {
                setPixel(j, i, (1 - t) * begin.z + t * end.z);
                p += ddx;
            }
            else {
                j += fx;
                setPixel(j, i, (1 - t) * begin.z + t * end.z);
                p += ds;
            }
        }
    }

    setPixel(end.x, end.y, end.z);
}

void Renderer::setPixel(const int& x, const int& y, const float& z) {
    auto window = window_manager::Window::getInstance();
    auto camera = Camera::getInstance();
    if (x >= 0 && y >= 0 && x < window->_width && y < window->_height &&
        _z_buffer[y * _width + x] <= z/** && z >= camera->getZNear() && z <= camera->getZFar() **/) {
        window->_buffer2[((_width * y + x) << 2) | 0] = _r;
        window->_buffer2[((_width * y + x) << 2) | 1] = _g;
        window->_buffer2[((_width * y + x) << 2) | 2] = _b;
        window->_buffer2[((_width * y + x) << 2) | 3] = _a;
        _z_buffer[y * _width + x] = z;
    }
}

void Renderer::triangle(math::Vector v1, math::Vector v2, math::Vector v3) {
    if (cullBackFace(v1, v2, v3)) {
        return;
    }

    auto e = v1 - v2;
    if (std::abs(e.x * v3.y - e.y * v3.x) < EPSILON) {
        this->line(v1, v2); this->line(v2, v3);
        return;
    }

    if (v1.y > v2.y) {
        std::swap(v1, v2);
    }
    if (v1.y > v3.y) {
        std::swap(v1, v3);
    }
    if (v2.y > v3.y) {
        std::swap(v2, v3);
    }

    float height = v3.y - v1.y, sub_height = v2.y - v1.y + 1;
    auto inc1 = (v3.x - v1.x) * (1.0f / height), inc2 = (v2.x - v1.x) * (1.0f / sub_height);
    auto sd1 = v1, sd2 = v1;
    for (int i = static_cast<int>(v1.y); i <= static_cast<int>(v2.y); ++i) {
        sd1.y = sd2.y = i;
        int min = std::min(sd1.x, sd2.x), max = std::max(sd1.x, sd2.x);
        for (int j = min; j <= max; ++j) {
            float u = ((j - v1.x) * (v2.y - v1.y) - (i - v1.y) * (v3.x - v1.x))
                    / ((v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x)),
                  v = ((j - v1.x) * (v2.y - v1.y) - (i - v1.y) * (v2.x - v1.x))
                    / ((v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x)),
                  w = 1 - u - v;
            setPixel(j, i, v1.z * w + v2.z * u + v3.z * v);
        }
        sd1.x += inc1; sd2.x += inc2;
    }

    sub_height = v3.y - v2.y + 1;
    inc2 = (v3.x - v2.x) * (1.0f / sub_height);
    sd2 = v2;
    for (int i = static_cast<int>(v2.y); i <= static_cast<int>(v3.y); ++i) {
        sd1.y = sd2.y = i;
        int min = std::min(sd1.x, sd2.x), max = std::max(sd1.x, sd2.x);
        for (int j = min; j <= max; ++j) {
            float u = ((j - v1.x) * (v2.y - v1.y) - (i - v1.y) * (v3.x - v1.x))
                      / ((v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x)),
                    v = ((j - v1.x) * (v2.y - v1.y) - (i - v1.y) * (v2.x - v1.x))
                        / ((v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x)),
                    w = 1 - u - v;
            setPixel(j, i, v1.z * w + v2.z * u + v3.z * v);
        }
        sd1.x += inc1; sd2.x += inc2;
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
    // TODO: failed
}

void Renderer::triangleWithTexture(math::Vector v1, math::Vector v2, math::Vector v3,
                             const math::Vector& tv1, const math::Vector& tv2, const math::Vector& tv3) {
    if (cullBackFace(v1, v2, v3) || _texture == nullptr) {
        return;
    }

    auto e = v1 - v2;
    if (std::abs(e.x * v3.y - e.y * v3.x) < EPSILON) {
        this->line(v1, v2); this->line(v2, v3);
        return;
    }

    if (v1.y > v2.y) {
        std::swap(v1, v2);
    }
    if (v1.y > v3.y) {
        std::swap(v1, v3);
    }
    if (v2.y > v3.y) {
        std::swap(v2, v3);
    }

    float height = v3.y - v1.y, sub_height = v2.y - v1.y + 1;
    auto inc1 = (v3.x - v1.x) * (1.0f / height), inc2 = (v2.x - v1.x) * (1.0f / sub_height);
    auto sd1 = v1, sd2 = v1;
    auto texture_size = math::Vector{_texture->get_width(), _texture->get_height(), 0};
    for (int i = std::round(v1.y); i <= std::round(v2.y); ++i) {
        sd1.y = sd2.y = i;
        int min = std::min(std::round(sd1.x), std::round(sd2.x)), max = std::max(std::round(sd1.x), std::round(sd2.x));
        for (int j = min; j <= max; ++j) {
            float u = (-(j - v2.x) * (v3.y - v2.y) + (i - v2.y) * (v3.x - v2.x))
                      / (-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x)),
                    v = (-(j - v3.x) * (v1.y - v3.y) + (i - v3.y) * (v1.x - v3.x))
                        / (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x)),
                    w = 1 - u - v;
            auto tv = tv1 * u + tv2 * v + tv3 * w;
            auto color = _texture->get(texture_size.x * tv.x, texture_size.y * tv.y);
            setColor(color.r, color.g, color.b, color.a);
            setPixel(j, i, v1.z * u + v2.z * v + v3.z * w);
        }
        sd1.x += inc1; sd2.x += inc2;
    }

    sub_height = v3.y - v2.y + 1;
    inc2 = (v3.x - v2.x) * (1.0f / sub_height);
    sd2 = v2;
    for (int i = std::round(v2.y); i <= std::round(v3.y); ++i) {
        sd1.y = sd2.y = i;
        int min = std::min(std::round(sd1.x), std::round(sd2.x)), max = std::max(std::round(sd1.x), std::round(sd2.x));
        for (int j = min; j <= max; ++j) {
            float u = (-(j - v2.x) * (v3.y - v2.y) + (i - v2.y) * (v3.x - v2.x))
                      / (-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x)),
                    v = (-(j - v3.x) * (v1.y - v3.y) + (i - v3.y) * (v1.x - v3.x))
                        / (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x)),
                    w = 1 - u - v;
            auto tv = tv1 * u + tv2 * v + tv3 * w;
            auto color = _texture->get(texture_size.x * tv.x, texture_size.y * tv.y);
            setColor(color.r, color.g, color.b, color.a);
            setPixel(j, i, v1.z * u + v2.z * v + v3.z * w);
        }
        sd1.x += inc1; sd2.x += inc2;
    }
}

} // namespace rendering