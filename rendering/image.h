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

#ifndef SEELE_IMAGE_H
#define SEELE_IMAGE_H

#include <string>

namespace rendering {

class Image {
public:
    Image(std::string filename, const size_t& width, const size_t& height);
    ~Image();

    void putPixel(int x, int y, unsigned char r, unsigned g, unsigned char b);
    void flush();
private:
    size_t _width, _height;
    std::string _filename;
    unsigned char* _buffer;
};

} // namespace rendering

#endif //SEELE_IMAGE_H
