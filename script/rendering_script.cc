/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file rendering_script.cc

#include "rendering_script.h"

#include "LuaBridge/LuaBridge.h"

namespace script {

RenderingScript::RenderingScript(const std::string& filename) {
    _state = luaL_newstate();
    luaL_openlibs(_state);
    auto err = luaL_dofile(_state, filename.c_str());

    if (err) {
        std::cerr << "can't open file " << filename << std::endl;
    }
}

void RenderingScript::execute() {
    _thread = std::thread{
        [&](){
            auto create_function = luabridge::getGlobal(_state, "onCreate");
            if (create_function.isFunction()) {
                create_function();
                //TODO:
            }
        }
    };

    _thread.join();
}

} // namespace script