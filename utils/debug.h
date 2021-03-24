/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file debug.h

#ifndef SEELE_DEBUG_H
#define SEELE_DEBUG_H

#include <string>
#include <iostream>

namespace utils {

class Debug {
public:
    static void terminate(const std::string& content, const int& error = -1);

    template<typename T>
    static void log(T t) {
        std::cout << t << std::endl;
    }

    template<typename T>
    static void logErr(T t) {
        std::cerr << t << std::endl;
    }
private:
};

} // namespace utils

#endif //SEELE_DEBUG_H
