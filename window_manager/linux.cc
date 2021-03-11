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

#ifdef PLATFORM_LINUX

#include "linux.h"

#include <cstring>
#include <thread>

namespace window_manager {

Window* Window::getInstance(const size_t& width, const size_t& height, const std::string& title) {
    if (_instance == nullptr) {
        _instance = new(std::nothrow) Window(width, height, title);

        if (_instance == nullptr) {
            Debug::terminate("can't create window instance.");
        }
    }

    return _instance;
}

void Window::destroyInstance() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void Window::render() {
    _timer = std::chrono::system_clock::now();

    while (!_closing) {
        _draw_func();
        draw();

        time_type now = std::chrono::system_clock::now();
        float duration = (now - _timer).count();
        if (duration <= 0.001) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(30ms);
        }

        pollEvent();
    }
}

Window::Window(const size_t& width, const size_t& height, const std::string& title) : _buffer(nullptr), _closing(false), _width(width), _height(height) {
    openDisplay();
    _screen = XDefaultScreen(_display);
    createWindow(title);
    createImage();

    XSaveContext(_display, _handle, _context, reinterpret_cast<XPointer>(this));
    XMapWindow(_display, _handle);
    XFlush(_display);
}

Window::~Window() {
    XUnmapWindow(_display, _handle);
    XDeleteContext(_display, _handle, _context);

    _image->data = nullptr;
    XDestroyImage(_image);
    XDestroyWindow(_display, _handle);
    XFlush(_display);

    delete _buffer;
    _buffer = nullptr;
}

void Window::openDisplay() {
    _display = XOpenDisplay(nullptr);
    if (_display == nullptr) {
        Debug::terminate("can't open display in X11.");
    }

    _protocols = XInternAtom(_display, "WM_PROTOCOLS", true);
    _delete_ptc = XInternAtom(_display, "WM_DELETE_WINDOW", true);

    if (_protocols == None || _delete_ptc == None) {
        Debug ::terminate("can't get delete window protocol!");
    }

    _context = XUniqueContext();
}

void Window::createWindow(const std::string& title) {
    auto border = XWhitePixel(_display, _screen);
    auto background = XBlackPixel(_display, _screen);
    auto root = XRootWindow(_display, _screen);

    _handle = XCreateSimpleWindow(_display, root, 0, 0, _width, _height, 0, border, background);

    auto size_hints = XAllocSizeHints();
    size_hints->flags = PMinSize | PMaxSize;
    size_hints->max_height = size_hints->min_height = _height;
    size_hints->max_width = size_hints->min_width = _width;
    XSetWMNormalHints(_display, _handle, size_hints);
    XFree(size_hints);

    auto class_hint = XAllocClassHint();
    class_hint->res_name = class_hint->res_class = const_cast<char*>(title.c_str());
    XSetClassHint(_display, _handle, class_hint);
    XFree(class_hint);

    XSelectInput(_display, _handle, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

    auto atom = XInternAtom(_display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(_display, _handle, &atom, 1);
}

void Window::createImage() {
    auto depth = XDefaultDepth(_display, _screen);
    auto visual = XDefaultVisual(_display, _screen);

    auto buffer_size = sizeof(unsigned char) * _width * _height * 4;
    _buffer = static_cast<unsigned char*>(::operator new(buffer_size));
    std::memset(_buffer, 0, buffer_size);

    _image = XCreateImage(_display, visual, depth, ZPixmap, 0,
                          reinterpret_cast<char*>(_buffer), _width, _height, 32, 0);
}

void Window::pollEvent() {
    int count = XPending(_display);
    while (count > 0) {
        XEvent event;
        XNextEvent(_display, &event);
        processEvent(event);
        --count;
    }

    XFlush(_display);
}

void Window::processEvent(const XEvent& event) {
    switch (event.type) {
        case ClientMessage:
            processClientMessage(event.xclient);
            break;
        default:
            break;
    }
}

void Window::processClientMessage(const XClientMessageEvent& event) {
    if (event.message_type == _protocols) {
        auto protocol = event.data.l[0];
        if (protocol == _delete_ptc) {
            _closing = true;
        }
    }
}

void Window::draw() {
    // TODO:
    GC gc = XDefaultGC(_display, _screen);
}

} // namespace window_manager

#endif // PLATFORM_LINUX
