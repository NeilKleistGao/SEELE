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

#include "utils/debug.h"
#include "rendering/renderer.h"

namespace model {

Model::Model(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (fp == nullptr) {
        utils::Debug::terminate("can't load model " + filename);
    }

    std::string cmd, data;
    bool is_cmd = false, split = false;
    while (!std::feof(fp)) {
        char c = fgetc(fp);
        if (c == ' ' && (!is_cmd || split)) {
            if (!is_cmd) {
                is_cmd = true;
            }
            else {
                processData(cmd, data);
                data = "";
            }
        }
        else if (!is_cmd && std::isalpha(c)) {
            cmd += c;
        }
        else if (c == '#') {
            while (!std::feof(fp) && c != '\n') {
                c = fgetc(fp);
            }
        }
        else if (c == '\n' || c == '\r') {
            if (is_cmd) {
                processData(cmd, data);
            }

            is_cmd = false;
            split = false;
            cmd = "";
            data = "";
        }
        else {
            if (c == '/') {
                split = true;
            }
            data += c;
        }
    }

    fclose(fp);
}

void Model::draw() {
    auto renderer = rendering::Renderer::getInstance();
    for (const auto& f : _faces) {
        const auto v0 = _vertex[std::get<0>(f) - 1];
        const auto v1 = _vertex[std::get<1>(f) - 1];
        const auto v2 = _vertex[std::get<2>(f) - 1];

        renderer->line(v0, v1); renderer->line(v1, v2); renderer->line(v2, v0);
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

        float x = fromString<float>(data.substr(0, pos1)),
                y = fromString<float>(data.substr(pos1 + 1, pos2 - pos1 - 1)),
                z = fromString<float>(data.substr(pos2 + 1));
        _vertex.emplace_back(x + 100, y + 100, z);
    }
    else if (cmd == "f") {
        int pos1, pos2;
        pos1 = data.find_first_of('/'), pos2 = data.find_last_of('/');

        if (pos1 == -1 || pos2 == -1) {
            return;
        }

        int x = fromString<int>(data.substr(0, pos1)),
                y = fromString<int>(data.substr(pos1 + 1, pos2 - pos1 - 1)),
                z = fromString<int>(data.substr(pos2 + 1));

        _faces.emplace_back(x, y, z);
    }
}

} // namespace model