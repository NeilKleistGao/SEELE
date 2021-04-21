/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file camera.h

#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace rendering {
Camera* Camera::_instance = nullptr;

Camera* Camera::getInstance() {
    if (_instance == nullptr) {
        _instance = new Camera{};
    }

    return _instance;
}

void Camera::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void Camera::init(const glm::vec3& position, const glm::vec3& look_at, const glm::vec4& perspective) {
    _view_point = glm::mat4 {1.0f};
    _view_point = glm::translate(_view_point, -position);

    auto theta = glm::dot(glm::normalize(look_at), glm::vec3 {0, 0, -1});
    _view_point = glm::rotate(_view_point, theta, glm::vec3{0, 0, -1});

    _perspective = {
        2 * perspective.z / perspective.x, 0, 0, 0,
        0, 2 * perspective.z / perspective.y, 0, 0,
        0, 0, - (perspective.z + perspective.w) / (perspective.w - perspective.z), -2 * perspective.z * perspective.w / (perspective.w - perspective.z),
        0, 0, -1, 0
    };
}

glm::vec4 Camera::viewPointTransform(const glm::vec4& vec) {
    return _view_point * vec;
}

glm::vec4 Camera::perspectiveTransform(const glm::vec4& vec) {
    return _perspective * vec;
}

Camera::Camera() : _perspective(1.0f), _view_point(1.0f) {

}

Camera::~Camera() = default;

} // namespace rendering
