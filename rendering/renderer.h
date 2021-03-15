/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file renderer.h

#ifndef SEELE_RENDERER_H
#define SEELE_RENDERER_H

#include "utils/debug.h"
#include "utils/registration.h"

namespace rendering {

class Renderer {
public:
    static Renderer* getInstance();
    static void destroyInstance();

    void clearWithColor(const unsigned char& r, const unsigned char& g, const unsigned char& b);
private:
    using Debug = utils::Debug;

    Renderer() = default;
    ~Renderer() = default;

    static Renderer* _instance;
};

} // namespace rendering

#endif //SEELE_RENDERER_H
