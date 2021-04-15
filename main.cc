/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file main.cc

#include <chrono>
#include <iomanip>

#include "cxxopts/cxxopts.hpp"
#include "script/rendering_script.h"

int main(int argc, char* argv[]) {
    cxxopts::Options options{"SEELE", "Software-rEndEring Laboratorial Engine"};

    options.add_options()
    ("s,script", "Rendering Script", cxxopts::value<std::string>())
    ("r, raytracing", "Enable Ray Tracing", cxxopts::value<bool>()->default_value("false"));

    try {
        auto result = options.parse(argc, argv);
        bool using_raytracing = result["raytracing"].as<bool>();
        std::string script_path = result["script"].as<std::string>();

        if (using_raytracing) {
            std::cerr << "not support for ray tracing yet!" << std::endl;
        }
        else {
            auto rs = new script::RenderingScript{script_path};

            auto begin = std::chrono::system_clock::now();
            rs->execute();
            auto end = std::chrono::system_clock::now();

            std::chrono::duration<double> diff = end - begin;
            double length = diff.count();
            std::cout << "rendering finished, using " << std::setw(4) << length << "s." << std::endl;
        }
    }
    catch (...) {
        std::cerr << options.help() << std::endl;
    }

    return 0;
}