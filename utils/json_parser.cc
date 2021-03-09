/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file json_parser.h

#include "json_parser.h"

#include <cstdio>

namespace utils {

rapidjson::Document JsonParser::parseDocument(const std::string& filename) {
    rapidjson::Document doc;
    std::string content = JsonParser::readAll(filename);
    doc.Parse(content.c_str());
    return doc;
}

std::string JsonParser::readAll(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (fp == nullptr) {
        return "";
    }

    std::string content;
    while (!feof(fp)) {
        char c = fgetc(fp);
        if (c != EOF) {
            content += c;
        }
    }

    fclose(fp);
    return content;
}

} // namespace utils
