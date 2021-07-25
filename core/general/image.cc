/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file image.cc

#include "image.h"

#include <utility>
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb/stb_image_write.h"

namespace core::general {

Image::Image(std::string filename, int width, int height)
    : _filename(std::move(filename)), _width(width), _height(height) {
    _buffer = new unsigned char[_width * _height * CHANNEL_SIZE];
}

Image::~Image() {
    this->flush();
    delete [] _buffer;
    _buffer = nullptr;
}

void Image::flush() {
    stbi_write_png(_filename.c_str(), _width, _height, CHANNEL_SIZE, _buffer, 0);
}

void Image::putPixel(int x, int y, float r, float g, float b) {
    int start = (y * _width + x) * CHANNEL_SIZE;
    r = gamma2(r / 255.0f) * 255.0f; g = gamma2(g / 255.0f) * 255.0f; b = gamma2(b / 255.0f) * 255.0f;
    _buffer[start] = static_cast<unsigned char>(r);
    _buffer[start + 1] = static_cast<unsigned char>(g);
    _buffer[start + 2] = static_cast<unsigned char>(b);
}

float Image::gamma2(float c) {
    c = std::sqrt(c);
    if (c > 1.0f) {
        c = 1.0f;
    }
    if (c < 0.0f) {
        c = 0.0f;
    }

    return c;
}

} // namespace core::general
