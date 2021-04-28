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
#include "core/rasterization/rasterization_renderer.h"

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
        for (int i = 0; i < mesh.Vertices.size(); i += 3) {
            ShaderDataList v2f[3];
            for (int j = 0; j < 3; ++j) {
                int index = mesh.Indices[i + j];
                auto v = mesh.Vertices[index];

                glm::vec3 position = {v.Position.X, v.Position.Y, v.Position.Z};
                glm::vec2 text = {v.TextureCoordinate.X, v.TextureCoordinate.Y};
                glm::vec3 normal = {v.Normal.X, v.Normal.Y, v.Normal.Z};

                ShaderDataList appdata;
                appdata.emplace_back(); appdata.emplace_back(); appdata.emplace_back();
                appdata[0].dimension = 3; appdata[0].interpolation = true; appdata[0].vec3 = position;
                appdata[1].dimension = 2; appdata[1].interpolation = true; appdata[1].vec2 = text;
                appdata[2].dimension = 3; appdata[2].interpolation = true; appdata[2].vec3 = normal;

                v2f[j] = v_shader->onVertex(appdata);
            }

            const glm::vec3 v1 = v2f[0][0].vec3, v2 = v2f[1][0].vec3, v3 = v2f[2][0].vec3;

            int min_x = std::min(std::min(v1.x, v2.x), v3.x);
            int min_y = std::min(std::min(v1.y, v2.y), v3.y);
            int max_x = std::max(std::max(v1.x, v2.x), v3.x);
            int max_y = std::max(std::max(v1.y, v2.y), v3.y);

            for (int i = min_x; i <= max_x; ++i) {
                for (int j = min_y; j <= max_y; ++j) {
                    float alpha = ((i - v3.x)*(v1.y - v3.y) - (v1.x - v3.x)*(j - v3.y)) /
                                  ((v2.x - v3.x)*(v1.y - v3.y) - (v1.x - v3.x)*(v2.y - v3.y)),
                            beta = ((j - v3.y)*(v1.x - v3.x) - (v1.y - v3.y)*(i - v3.x)) /
                                   ((v2.y - v3.y)*(v1.x - v3.x) - (v1.y - v3.y)*(v2.x - v3.x)),
                            gamma = 1 - alpha - beta;
                    if (alpha >= 0.0f && alpha <= 1.0f && beta >= 0.0f && beta <= 1.0f && gamma >= 0.0f && gamma <= 1.0f) {
                        auto f_data = interpolate(v2f[0], v2f[1], v2f[2],
                                                  alpha, beta, gamma);
                        auto color = f_shader->onFragment(f_data);
                        renderer->putPixel(f_data[0].vec3, color);
                    }
                }
            }
        }
    }
}

bool TriMesh::isInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& point) {
    auto e1 = v1 - v3, t1 = point - v3;
    auto e2 = v2 - v1, t2 = point - v1;
    auto e3 = v3 - v2, t3 = point - v2;

    auto c1 = e1.x * t1.y - e1.y * t1.x;
    auto c2 = e2.x * t2.y - e2.y * t2.x;
    auto c3 = e3.x * t3.y - e3.y * t3.x;

    return (c1 >= 0.0f && c2 >= 0.0f && c3 >= 0.0f) || (c1 <= 0.0f && c2 <= 0.0f && c3 <= 0.0f);
}

TriMesh::ShaderDataList TriMesh::interpolate(
        const ShaderDataList& v1,
        const ShaderDataList& v2,
        const ShaderDataList& v3,
        float alpha, float beta, float gamma
) {
    ShaderDataList res;
    int total = v1.size();
    for (int i = 0; i < total; ++i) {
        ShaderDataItem item = v1[i];
        if (item.interpolation) {
            switch (item.dimension) {
                case 1:
                    item.f = v1[i].f * alpha + v2[i].f * beta + v3[i].f * gamma;
                    break;
                case 2:
                    item.vec2 = v1[i].vec2 * alpha + v2[i].vec2 * beta + v3[i].vec2 * gamma;
                    break;
                case 3:
                    item.vec3 = v1[i].vec3 * alpha + v2[i].vec3 * beta + v3[i].vec3 * gamma;
                    break;
                case 4:
                    item.vec4 = v1[i].vec4 * alpha + v2[i].vec4 * beta + v3[i].vec4 * gamma;
                    break;
            }
        }

        res.push_back(item);
    }

    return res;
}

} // namespace components