/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file model.h

#ifndef SEELE_MODEL_H
#define SEELE_MODEL_H

#include <string>
#include <vector>
#include <tuple>

#include "math/vector.h"
#include "utils/registration.h"

namespace model {

class Model {
public:
    explicit Model(const std::string& filename);

    void draw();
private:
    using t_face = std::tuple<int, int, int>;
    using q_face = std::tuple<int, int, int>;

    void processData(const std::string& cmd, const std::string& data);

    template<typename T>
    T fromString(std::string&& str) {
        std::stringstream stream;
        stream << str;
        T t;
        stream >> t;
        return t;
    }

    std::vector<math::Vector> _vertex;
    std::vector<t_face> _faces;
};

SEELE_REGISTRATION(Model) {
    luabridge::getGlobalNamespace(script::RenderingScript::getInstance()->getState())
        .beginNamespace("seele")
            .beginClass<Model>("Model")
                .addConstructor<void (*) (const std::string&)>()
                .addFunction("draw", &Model::draw)
            .endClass()
        .endNamespace();
}

} // namespace model

#endif //SEELE_MODEL_H
