/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file material.h

#ifndef SEELE_MATERIAL_H
#define SEELE_MATERIAL_H

#include "glm/glm.hpp"

namespace assets {

struct Material {
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;

    Material() = default;

    Material(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks)
    : Ka(ka), Kd(kd), Ks(ks) {}
};

} // namespace assets

#endif //SEELE_MATERIAL_H
