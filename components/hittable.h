/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file hittable.h

#ifndef SEELE_HITTABLE_H
#define SEELE_HITTABLE_H

namespace components {

class Hittable {
public:
    virtual bool intersect(const core::raytracing::Ray& ray, core::raytracing::HitRecord& record, float max) const = 0;
};

} // namespace components

#endif //SEELE_HITTABLE_H
