/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file i_element.h

#ifndef SEELE_I_ELEMENT_H
#define SEELE_I_ELEMENT_H

#include "glm/glm.hpp"

namespace rendering {

class IElement {
public:
    IElement() = default;
    virtual ~IElement() = default;

    virtual void assemble() = 0;
    virtual void render() = 0;
    virtual glm::vec4 modelTransform(const glm::vec4& vec) = 0;
protected:
};

} // namespace rendering

#endif //SEELE_I_ELEMENT_H
