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

namespace rendering {
Renderer* Renderer::_instance = nullptr;

Renderer* Renderer::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) Renderer();
        if (_instance == nullptr) {
            Debug::terminate("can't create renderer instance.");
        }
    }

    auto win = window_manager::Window::getInstance();
    _instance->_width = win->_width; _instance->_height = win->_height;

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
    auto size = window->_width * window->_height;
    for (int i = 0; i < size; i++) {
        window->_buffer2[(i << 2)] = r;
        window->_buffer2[(i << 2) | 1] = g;
        window->_buffer2[(i << 2) | 2] = b;
        window->_buffer2[(i << 2) | 3] = 255;
    }
}

void Renderer::line(const math::Vector& begin, const math::Vector& end) {
    int dx = std::fabs(end.x - begin.x), dy = std::fabs(end.y - begin.y);
    int fx = (end.x - begin.x) / dx, fy = (end.y - begin.y) / dy;

    if (dx == 0 && dy == 0) {
        setPixel(begin.x, begin.y);
        return;
    }
    else if (dx == 0) {
        for (int i = static_cast<int>(begin.y); i != static_cast<int>(end.y); i += fy) {
            setPixel(begin.x, i);
        }
    }
    else if (dy == 0) {
        for (int i = static_cast<int>(begin.x); i != static_cast<int>(end.x); i += fx) {
            setPixel(i, begin.y);
        }
    }
    else if (dx == dy) {
        for (int i = static_cast<int>(begin.x), j = static_cast<int>(begin.y);
            i != static_cast<int>(end.x); i += fx, j += fy) {
            setPixel(i, j);
        }
    }
    else if (dx > dy) {
        int p = 2 * dy - dx, ddy = 2 * dy, ds = 2 * (dy - dx);
        setPixel(begin.x, begin.y);
        for (int i = static_cast<int>(begin.x) + 1, j = static_cast<int>(begin.y); i != static_cast<int>(end.x); i += fx) {
            if (p < 0) {
                setPixel(i, j);
                p += ddy;
            }
            else {
                j += fy;
                setPixel(i, j);
                p += ds;
            }
        }
    }
    else {
        int p = 2 * dx - dy, ddx = 2 * dx, ds = 2 * (dx - dy);
        setPixel(begin.x, begin.y);
        for (int i = static_cast<int>(begin.y) + 1, j = static_cast<int>(begin.x); i != static_cast<int>(end.y); i += fy) {
            if (p < 0) {
                setPixel(j, i);
                p += ddx;
            }
            else {
                j += fx;
                setPixel(j, i);
                p += ds;
            }
        }
    }
}

void Renderer::setPixel(const int& x, const int& y) {
    auto window = window_manager::Window::getInstance();
    if (x >= 0 && y >= 0 && x < window->_width && y < window->_height) {
        window->_buffer2[((window->_width * y + x) << 2) | 0] = _r;
        window->_buffer2[((window->_width * y + x) << 2) | 1] = _g;
        window->_buffer2[((window->_width * y + x) << 2) | 2] = _b;
        window->_buffer2[((window->_width * y + x) << 2) | 3] = _a;
    }
}

void Renderer::triangle(math::Vector v1, math::Vector v2, math::Vector v3) {
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

    if (v2.y - v1.y <= 1.0f) {
        float height = v3.y - v1.y;
        auto inc1 = (v3 - v1) * (1.0f / height), inc2 = (v3 - v2) * (1.0f / height);
        auto sd1 = v1, sd2 = v2;
        for (int i = static_cast<int>(v1.y); i < static_cast<int>(v3.y); ++i) {
            sd1 += inc1; sd2 += inc2;
            this->line(sd1, sd2);
        }
    }
    else if (v3.y - v2.y <= 1.0f) {
        float height = v3.y - v1.y;
        auto inc1 = (v2 - v1) * (1.0f / height), inc2 = (v3 - v1) * (1.0f / height);
        auto sd1 = v1, sd2 = v1;
        for (int i = static_cast<int>(v1.y); i < static_cast<int>(v3.y); ++i) {
            sd1 += inc1; sd2 += inc2;
            this->line(sd1, sd2);
        }
    }
    else {
        float height = v3.y - v1.y, sub_height = v2.y - v1.y;
        auto inc1 = (v3 - v1) * (1.0f / height), inc2 = (v2 - v1) * (1.0f / sub_height);
        auto sd1 = v1, sd2 = v1;
        for (int i = static_cast<int>(v1.y); i < static_cast<int>(v2.y); ++i) {
            sd1 += inc1; sd2 += inc2;
            this->line(sd1, sd2);
        }

        sub_height = v3.y - v2.y;
        inc1 = (v3 - v1) * (1.0f / height), inc2 = (v3 - v2) * (1.0f / sub_height);
        sd1 = v2, sd2 = v2;
        for (int i = static_cast<int>(v2.y); i < static_cast<int>(v3.y); ++i) {
            sd1 += inc1; sd2 += inc2;
            this->line(sd1, sd2);
        }
    }
}

} // namespace rendering