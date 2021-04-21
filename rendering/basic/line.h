/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file line.h

#ifndef SEELE_LINE_H
#define SEELE_LINE_H

#include "glm/glm.hpp"
#include "../i_element.h"

namespace rendering::basic {

class Line : public IElement {
public:
    Line(const glm::vec3& begin, const glm::vec3& end);

    void render() override;
    void assemble() override;

    glm::vec4 modelTransform(const glm::vec4& vec) override;
private:
    glm::vec4 _begin;
    glm::vec4 _end;
protected:
};

} // namespace rendering::basic

#endif //SEELE_LINE_H
