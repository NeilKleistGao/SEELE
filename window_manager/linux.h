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

#include <iostream>
#include <exception>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include "utils/debug.h"

namespace window_manager {
class Window {
public:
    static Window* getInstance(const size_t& width = 800, const size_t& height = 600) {
        if (_instance == nullptr) {
            _instance = new(std::nothrow) Window(width, height);

            if (_instance == nullptr) {
                Debug::terminate("can't create window instance.");
            }
        }

        return _instance;
    }

    static void destroyInstance() {

    }

    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
    Window(Window&&) = delete;

    void render() {

    }

private:
    using Debug = utils::Debug;

    constexpr static char* TITLE = "Software-rendering ExtendiblE Laboratorial Engine";

    static Window* _instance;
    ::Window _handle;
    XImage* _image;
    Display* _display;
    XContext _content;

    bool _closing;

    Window(const size_t& width, const size_t& height) {
        openDisplay();
        createWindow(width, height);
        createImage();
    }

    ~Window() {

    }

    void openDisplay() {
        _display = XOpenDisplay(nullptr);
        if (_display == nullptr) {
            Debug::terminate("can't open display in X11.");
        }

        _content = XUniqueContext();
    }

    void createWindow(const size_t& width, const size_t& height) {
        auto screen = XDefaultScreen(_display);
        auto border = XWhitePixel(_display, screen);
        auto background = XBlackPixel(_display, screen);
        auto root = XRootWindow(_display, screen);

        _handle = XCreateSimpleWindow(_display, root, 0, 0, width, height, 0, border, background);

        auto size_hints = XAllocSizeHints();
        size_hints->flags = PMinSize | PMaxSize;
        size_hints->max_height = size_hints->min_height = height;
        size_hints->max_width = size_hints->min_width = width;
        XSetWMNormalHints(_display, _handle, size_hints);
        XFree(size_hints);

        auto class_hint = XAllocClassHint();
        class_hint->res_name = class_hint->res_class = TITLE;
        XSetClassHint(_display, _handle, class_hint);
        XFree(class_hint);

        XSelectInput(_display, _handle, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

        auto atom = XInternAtom(_display, "WM_DELETE_WINDOW", true);
        XSetWMProtocols(_display, _handle, &atom, 1);
    }

    void createImage() {

    }
};

} // namespace window_manager

#endif //SEELE_LINUX_H
