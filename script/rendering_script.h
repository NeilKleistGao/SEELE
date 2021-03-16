/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file rendering_script.h

#ifndef SEELE_RENDERING_SCRIPT_H
#define SEELE_RENDERING_SCRIPT_H

#include <filesystem>

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "utils/debug.h"

namespace script {

class RenderingScript {
public:
    static RenderingScript* getInstance();
    static void destroyInstance();

    void preload(const std::filesystem::path& path);
    void load(const std::string& filename);
    void update(const float& delta);

    inline lua_State* getState() {
        return _state;
    }
private:
    using Debug = utils::Debug;

    RenderingScript();
    ~RenderingScript();

    static RenderingScript* _instance;
    lua_State* _state;
};

} // namespace script

#endif //SEELE_RENDERING_SCRIPT_H
