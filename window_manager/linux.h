/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file linux.h

#ifndef SEELE_LINUX_H
#define SEELE_LINUX_H

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

namespace window_manager {
class Window {
public:
    Window() {

    }

    ~Window() {

    }

    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
    Window(Window&&) = delete;



private:
    ::Window _handle;
    XImage* _image;
    bool _closing;
};

} // namespace window_manager

#endif //SEELE_LINUX_H
