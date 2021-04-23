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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb/stb_image_write.h"

namespace core::general {

Image::Image(std::string  filename, int width, int height)
    : _filename(std::move(filename)), _width(width), _height(height) {
    _buffer = new unsigned char[_width * _height * CHANNEL_SIZE];
}

Image::~Image() {
    this->flush();
    delete [] _buffer;
    _buffer = nullptr;
}

void Image::flush() {
    stbi_write_jpg(_filename.c_str(), _width, _height, CHANNEL_SIZE, _buffer, 2);
}

} // namespace core::general
