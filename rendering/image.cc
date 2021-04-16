/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file image.h

#include "image.h"

#include <cstring>

namespace rendering {

Image::Image(std::string filename, const size_t& width, const size_t& height)
    : _buffer(nullptr), _filename(std::move(filename)), _width(width), _height(height) {
    _buffer = new unsigned char[width * height * 3];
    std::memset(_buffer, 0, sizeof(unsigned char) * width * height * 3)
}

Image::~Image() {
    flush();
    delete[] _buffer;
    _buffer = nullptr;
}

void Image::putPixel(int x, int y, unsigned char r, unsigned g, unsigned char b) {
    if (x > -1 && y > -1 && x < _width && y < _height) {
        y = _height - y - 1;
        auto index = y * _width + x;
        _buffer[index] = r;
        _buffer[index] = g;
        _buffer[index] = b;
    }
}

} // namespace rendering