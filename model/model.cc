/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file model.cc

#include "model.h"

#include <cstdio>
#include <cctype>
#include <filesystem>

#include "utils/debug.h"
#include "rendering/renderer.h"
#include "rendering/camera.h"
#include "math/matrix.h"

namespace model {

Model::Model(const std::string& filename, const std::string& texture) : _position(), _scale(1, 1, 1), _rotation(0, 0, 0) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (fp == nullptr) {
        utils::Debug::terminate("can't load model " + filename);
    }

    std::string cmd, data;
    bool is_cmd = false;
    while (!std::feof(fp)) {
        char c = fgetc(fp);
        if (c == ' ') {
            if (!is_cmd) {
                is_cmd = true;
                if (cmd.length() > 1) {
                    fgetc(fp);
                }
            }
            else {
                data += c;
            }
        }
        else if (!is_cmd && std::isalpha(c)) {
            cmd += c;
        }
        else if (c == '#') {
            while (!std::feof(fp) && c != '\n') {
                c = fgetc(fp);
            }

            is_cmd = false;
            cmd = "";
            data = "";
        }
        else if (c == '\n' || c == '\r') {
            if (is_cmd) {
                processData(cmd, data);
            }

            is_cmd = false;
            cmd = "";
            data = "";
        }
        else {
            data += c;
        }
    }

    fclose(fp);
    loadTexture(texture);
}

void Model::draw() {
    auto renderer = rendering::Renderer::getInstance();
    auto camera = rendering::Camera::getInstance();

    math::Matrix trans = MVTransform();

    for (int i = 0; i < _faces.size(); ++i) {
        const auto f = _faces[i];
        const auto ti = _text_index[i];
        auto v0 = camera->perspectiveTransform(trans * _vertex[std::get<0>(f)]);
        v0.x *= renderer->getWidth(); v0.y *= renderer->getHeight();
        auto v1 = camera->perspectiveTransform(trans * _vertex[std::get<1>(f)]);
        v1.x *= renderer->getWidth(); v1.y *= renderer->getHeight();
        auto v2 = camera->perspectiveTransform(trans * _vertex[std::get<2>(f)]);
        v2.x *= renderer->getWidth(); v2.y *= renderer->getHeight();
        const auto tv0 = _text_coord[std::get<0>(ti)];
        const auto tv1 = _text_coord[std::get<1>(ti)];
        const auto tv2 = _text_coord[std::get<2>(ti)];

        renderer->triangleWithTexture(v0, v1, v2, tv0, tv1, tv2);
    }
}

void Model::processData(const std::string& cmd, const std::string& data) {
    std::stringstream stream;

    if (cmd == "v") {
        int pos1, pos2;
        pos1 = data.find_first_of(' '), pos2 = data.find_last_of(' ');

        if (pos1 == -1 || pos2 == -1) {
            return;
        }

        auto x = fromString<float>(data.substr(0, pos1)),
                y = fromString<float>(data.substr(pos1 + 1, pos2 - pos1 - 1)),
                z = fromString<float>(data.substr(pos2 + 1));
        _vertex.emplace_back(x, y, z);
    }
    else if (cmd == "f") {
        int pos1, pos2;
        pos1 = data.find_first_of(' '), pos2 = data.find_last_of(' ');

        if (pos1 == -1 || pos2 == -1) {
            return;
        }

        std::string temp_x = data.substr(0, pos1),
                    temp_y = data.substr(pos1 + 1, pos2 - pos1 - 1),
                    temp_z = data.substr(pos2 + 1);
        auto xp1 = temp_x.find_first_of('/'), xp2 = temp_x.find_last_of('/'),
             yp1 = temp_y.find_first_of('/'), yp2 = temp_y.find_last_of('/'),
             zp1 = temp_z.find_first_of('/'), zp2 = temp_z.find_last_of('/');

        int x = fromString<int>(temp_x.substr(0, xp1)),
                y = fromString<int>(temp_y.substr(0, yp1)),
                z = fromString<int>(temp_z.substr(0, zp1));

        _faces.emplace_back(x - 1, y - 1, z - 1);

        x = fromString<int>(temp_x.substr(xp1 + 1, xp2 - xp1)),
        y = fromString<int>(temp_y.substr(yp1 + 1, yp2 - yp1)),
        z = fromString<int>(temp_z.substr(zp1 + 1, zp2 - zp1));

        _text_index.emplace_back(x - 1, y - 1, z - 1);
    }
    else if (cmd == "vt") {
        int pos1, pos2;
        pos1 = data.find_first_of(' '), pos2 = data.find_last_of(' ');

        if (pos1 == -1 || pos2 == -1) {
            return;
        }

        auto x = fromString<float>(data.substr(0, pos1)),
                y = fromString<float>(data.substr(pos1 + 1, pos2 - pos1 - 1)),
                z = fromString<float>(data.substr(pos2 + 1));
        _text_coord.emplace_back(x, 1 - y, z);
    }
}

void Model::loadTexture(const std::string& texture) {
    if (texture.empty()) {
        return;
    }

    _texture = new TGAImage();
    if (!_texture->read_tga_file(texture.c_str())) {
        utils::Debug::terminate("Can't load texture " + texture);
    }

    rendering::Renderer::getInstance()->bindTexture(_texture);
}

math::Matrix Model::MVTransform() {
    auto camera = rendering::Camera::getInstance();

    math::Matrix trans{};
    /// model
    trans.scale(_scale)
            .rotate(math::Vector::X, _rotation.x)
            .rotate(math::Vector::Y, _rotation.y)
            .rotate(math::Vector::Z, _rotation.z)
            .move(_position);
    /// view
    auto cr = camera->getRotation();
    trans.move(camera->getPosition())
         .rotate(math::Vector::X, -cr.x)
         .rotate(math::Vector::Y, -cr.y)
         .rotate(math::Vector::Z, -cr.z);

    return trans;
}

} // namespace model