/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */


/// @file light_manager.h

#ifndef SEELE_LIGHT_MANAGER_H
#define SEELE_LIGHT_MANAGER_H

#include <list>

#include "light.h"
#include "utils/registration.h"

namespace light {

class LightManager {
public:
    static LightManager* getInstance();
    static void destroyInstance();

    inline void addLight(Light* lt) {
        _lights.push_back(lt);
    }

    math::Vector calculateLight(const math::Vector& position,
                                const math::Vector& normal,
                                const math::Vector& color,
                                const float& ambient,
                                const float& diffuse,
                                const float& specular);

private:
    LightManager();
    ~LightManager();

    static LightManager* _instance;
    std::list<Light*> _lights;
};

SEELE_REGISTRATION(LightManager) {
    luabridge::getGlobalNamespace(script::ScriptManager::getInstance()->getState())
        .beginNamespace("seele")
            .beginClass<LightManager>("LightManager")
                .addStaticFunction("getInstance", &LightManager::getInstance)
                .addFunction("addLight", &LightManager::addLight)
            .endClass()
        .endNamespace();
}

} // namespace light

#endif //SEELE_LIGHT_MANAGER_H
