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
#include <chrono>
#include <functional>
#include <utility>

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include "utils/debug.h"

namespace rendering {
class Renderer;
} // namespace rendering

namespace window_manager {
class Window {
public:
    static Window* getInstance(const size_t& width = 800, const size_t& height = 600, const std::string& title = "");

    static void destroyInstance();

    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
    Window(Window&&) = delete;

    void render();

    inline void setDrawFunction(std::function<void(const float&)> func) {
        _draw_func = std::move(func);
    }

    friend class rendering::Renderer;
private:
    using Debug = utils::Debug;
    using time_type = std::chrono::time_point<std::chrono::system_clock>;

    static Window* _instance;
    ::Window _handle;
    XImage* _image;
    Display* _display;
    XContext _context;
    int _screen;

    Atom _protocols;
    Atom _delete_ptc;

    unsigned char* _buffer1, *_buffer2;

    bool _closing;

    size_t _width, _height;

    time_type _timer;

    std::function<void(const float&)> _draw_func;

    Window(const size_t& width, const size_t& height, const std::string& title);

    ~Window();

    void openDisplay();

    void createWindow(const std::string& title);

    void createImage();

    void pollEvent();

    void processEvent(const XEvent& event);

    void processClientMessage(const XClientMessageEvent& event);

    void display();
};

} // namespace window_manager

#endif //SEELE_LINUX_H
