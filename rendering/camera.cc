/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file camera.cc

#include "camera.h"

#include <cmath>

#include "renderer.h"

namespace rendering {

Camera* Camera::_instance = nullptr;

Camera::Camera() : _position(0, 0, 0, 1),
    _rotation(0, 0, 0), _z_near(5), _z_far(-5), _camera_size(8, 6) {
    auto height = Renderer::getInstance()->getHeight();
    _fov_angle = std::atan2(height, 5) * 2;
}

Camera::~Camera() {
}

Camera* Camera::getInstance() {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) Camera();

        if (_instance == nullptr) {
            Debug::terminate("can't create camera instance.");
        }
    }

    return _instance;
}

math::Vector Camera::perspectiveTransform(const math::Vector& v) {
    return math::Vector{(2 * v.x * 5 / (10 - v.z) / _camera_size.x + 0.5f),
                        (2 * v.y * 5 / (10 - v.z) / _camera_size.y + 0.5f),
                        v.z, 1};
}

} // namespace rendering