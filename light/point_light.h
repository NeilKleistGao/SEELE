/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file point_light.h

#ifndef SEELE_POINT_LIGHT_H
#define SEELE_POINT_LIGHT_H

#include "light.h"
#include "utils/registration.h"

namespace light {

class PointLight : public Light {
public:
    PointLight(math::Vector position, float intensity);

    math::Vector calculateLight(const math::Vector& position,
                                const math::Vector& normal,
                                const math::Vector& color,
                                const float& ambient,
                                const float& diffuse,
                                const float& specular) override;
    ~PointLight() override = default;

    inline void setColor(const math::Vector& color) {
        _color = color;
    }
private:
    math::Vector _position;
    math::Vector _color;
    float _intensity;
};

SEELE_REGISTRATION(PointLight) {
    luabridge::getGlobalNamespace(script::ScriptManager::getInstance()->getState())
        .beginNamespace("seele")
            .deriveClass<PointLight, Light>("PointLight")
                .addConstructor<void(*)(math::Vector, float)>()
                .addFunction("setColor", &PointLight::setColor)
            .endClass()
        .endNamespace();
}

} // namespace light

#endif //SEELE_POINT_LIGHT_H
