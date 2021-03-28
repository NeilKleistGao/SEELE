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

#include "utils/debug.h"
#include "script_manager.h"

namespace script {

class RenderingScript {
public:
    static RenderingScript* getInstance();
    static void destroyInstance();

    void preload(const std::filesystem::path& path);
    void load(const std::string& filename);
    void update(const float& delta);

    template<typename T>
    T getGlobalVariable(const std::string& name) {
        luabridge::LuaRef ref = luabridge::getGlobal(ScriptManager::getInstance()->getState(), name.c_str());
        if (ref.isNil()) {
            return T();
        }
        else {
            return ref.cast<T>();
        }
    }

    template <typename T>
    void call(const std::string& name, const T& v) {
        luabridge::LuaRef ref = luabridge::getGlobal(ScriptManager::getInstance()->getState(), name.c_str());
        if (!ref.isFunction()) {
            return;
        }

        ref(v);
    }

    template <typename T, typename P>
    void call(const std::string& name, const T& v1, const P& v2) {
        luabridge::LuaRef ref = luabridge::getGlobal(ScriptManager::getInstance()->getState(), name.c_str());
        if (!ref.isFunction()) {
            return;
        }

        ref(v1, v2);
    }
private:
    using Debug = utils::Debug;

    RenderingScript();
    ~RenderingScript();

    static RenderingScript* _instance;
};

} // namespace script

#endif //SEELE_RENDERING_SCRIPT_H
