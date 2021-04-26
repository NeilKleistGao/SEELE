/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file transform.cc

#include "transform.h"

#include "glm/gtc/matrix_transform.hpp"
#include "core/general/renderer.h"

extern core::general::Renderer* renderer;

namespace components {

Transform::Transform() : _position(), _model(), _scale(1, 1, 1), _rotation() {
}

void Transform::updateModelMatrix() {
    _model = glm::mat4(1.0f);
    _model = glm::rotate(_model, _rotation.x, glm::vec3(1, 0, 0));
    _model = glm::rotate(_model, _rotation.y, glm::vec3(0, 1, 0));
    _model = glm::rotate(_model, _rotation.z, glm::vec3(0, 0, 1));
    _model = glm::scale(_model, _scale);
    _model = glm::translate(_model, _position);
}

} // namespace components