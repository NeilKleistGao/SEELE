/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file texture.h

#ifndef SEELE_TEXTURE_H
#define SEELE_TEXTURE_H

#include <string>

#include "glm/glm.hpp"

namespace assets {

class Texture {
public:
    explicit Texture(const std::string& filename);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = delete;

    glm::vec4 getColor(float x, float y) const;
private:
    int _width;
    int _height;
    int _channel;
    unsigned char* _image;
protected:
};

} // namespace assets

#endif //SEELE_TEXTURE_H
