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

#include "glm/gtc/matrix_transform.hpp"

namespace components {

Camera::Camera(const glm::vec3& look_from, const glm::vec3& look_at, const glm::vec3& vup, float fov, float width, float height)
: Transform(), _width(width), _height(height) {
    _position = look_from;

    auto theta = fov / 180.0f * 3.1415926535f;
    _focus = 1 / std::tan(theta / 2);

    _v = glm::mat4 {1.0f};
    _v = glm::translate(_v, -_position);

    auto x = glm::normalize(glm::cross(look_at, vup));
    auto temp = glm::mat4 {1.0f};
    temp[0][0] = x.x; temp[0][1] = vup.x; temp[0][2] = -look_at.x;
    temp[1][0] = x.y; temp[1][1] = vup.y; temp[1][2] = -look_at.y;
    temp[2][0] = x.z; temp[2][1] = vup.z; temp[2][2] = -look_at.z;
    _v = temp * _v;

    _p = glm::mat4 {0.0f};
    _p[0][0] = 2 * _focus / _width;
    _p[1][1] = 2 * _focus / _height;
    _p[2][2] = _p[2][3] = -1;
    _p[3][2] = -2 * _focus;

    _rtv = glm::mat4 {1.0f};
    _rtv = glm::translate(_rtv, _position);
    temp = glm::mat4 {1.0f};
    temp[0][0] = x.x; temp[0][1] = x.y; temp[0][2] = x.z;
    temp[1][0] = vup.x; temp[1][1] = vup.y; temp[1][2] = vup.z;
    temp[2][0] = -look_at.x; temp[2][1] = -look_at.y; temp[2][2] = -look_at.z;
    _rtv = _rtv * temp;
    _rtv = glm::transpose(_rtv);

    _rtp = glm::mat4 {0.0f};
    _rtp[0][0] = _width / (2 * _focus);
    _rtp[1][1] = _height / (2 * _focus);
    _rtp[3][2] = -1;
    _rtp[2][3] = -1 / (2 * _focus);
    _rtp[3][3] = 1 / (2 * _focus);
}

Camera::~Camera() = default;

glm::vec4 Camera::transform(const glm::vec4& vec) {
    auto temp =  _p * _v * vec;
    return {temp.x * (_width / 2.0f), temp.y * (_height / 2.0f), temp.z, temp.w};
}

} // namespace components