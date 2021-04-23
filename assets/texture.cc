/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file texture.cc

#include "texture.h"

#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace assets {

Texture::Texture(const std::string& filename) : _channel(), _height(), _width(), _image(nullptr) {
    _image = stbi_load(filename.c_str(), &_width, &_height, &_channel, 0);
}

Texture::~Texture() {
    delete _image;
    _image = nullptr;
}

glm::vec4 Texture::getColor(float x, float y) const {
    x = std::min(x, 1.0f);
    x = std::max(0.0f, x);
    y = std::min(y, 1.0f);
    y = std::max(0.0f, y);

    int ix = x * _width, iy = y * _height;
    int index = iy * _width + ix * _channel;

    if (_channel == 3) {
        return glm::vec4 {_image[index], _image[index + 1], _image[index + 2], 0};
    }
    else if (_channel == 4) {
        return glm::vec4 {_image[index], _image[index + 1], _image[index + 2], _image[index + 3]};
    }
    else {
        return glm::vec4 {};
    }
}

} // namespace assets