/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */


/// @file tri_mesh.cc

#include "tri_mesh.h"

/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */


/// @file tri_mesh.cc

#include "tri_mesh.h"

#include <utility>

#include "include/OBJLoader/OBJ_Loader.hpp"
#include "core/general/renderer.h"
#include "core/rasterization/rasterization_renderer.h"
#include "core/general/shader.h"

namespace components {

TriMesh::TriMesh(std::string filename, std::string vertex_shader, std::string fragment_shader)
        : Transform(), _object(nullptr),
          _fragment_shader(std::move(fragment_shader)),
          _vertex_shader(std::move(vertex_shader)) {
    _object = new objl::Loader();
    _object->LoadFile(std::move(filename));
}

TriMesh::~TriMesh() {
    if (_object != nullptr) {
        delete _object;
        _object = nullptr;
    }
}

void TriMesh::rasterize(core::rasterization::RasterizationRenderer* renderer) {
    updateModelMatrix();
    auto v_shader = renderer->getShader(_vertex_shader);
    auto f_shader = renderer->getShader(_fragment_shader);

    for (auto& mesh : _object->LoadedMeshes) {
        std::vector<core::general::Shader::ShaderDataItem> v2f[3];
        for (int i = 0; i < 3; ++i) {
            int index = mesh.Indices[i];
            auto v = mesh.Vertices[index];

            glm::vec3 position = {v.Position.X, v.Position.Y, v.Position.Z};
            glm::vec2 text = {v.TextureCoordinate.X, v.TextureCoordinate.Y};
            glm::vec3 normal = {v.Normal.X, v.Normal.Y, v.Normal.Z};

            std::vector<core::general::Shader::ShaderDataItem> appdata;
            appdata.emplace_back(); appdata.emplace_back(); appdata.emplace_back();
            appdata[0].dimension = 3; appdata[0].interpolation = true; appdata[0].vec3 = position;
            appdata[1].dimension = 2; appdata[1].interpolation = true; appdata[1].vec2 = text;
            appdata[2].dimension = 3; appdata[2].interpolation = true; appdata[2].vec3 = normal;

            v2f[i] = v_shader->onVertex(appdata);
        }

        // TODO:
    }
}

} // namespace components