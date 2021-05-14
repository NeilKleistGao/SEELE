/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file tri_mesh.h


#ifndef SEELE_TRI_MESH_H
#define SEELE_TRI_MESH_H

#include <string>
#include <map>

#include "transform.h"
#include "core/general/shader.h"
#include "assets/texture.h"

namespace objl {
class Loader;
} // namespace objl

namespace components {

class TriMesh : public Transform {
public:
    TriMesh(std::string filename, const luabridge::LuaRef& vertex_shader, const luabridge::LuaRef& fragment_shader);
    ~TriMesh() override;

    void rasterize(core::rasterization::RasterizationRenderer* renderer, int pass) override;
private:
    using ShaderDataItem = core::general::Shader::ShaderDataItem;
    using ShaderDataList = std::vector<ShaderDataItem>;
    objl::Loader* _object;

    luabridge::LuaRef _vertex_shader;
    luabridge::LuaRef _fragment_shader;

    std::map<std::string, assets::Texture*> _textures;

    static bool isInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& point);
    static ShaderDataList interpolate(
            const ShaderDataList& v1,
            const ShaderDataList& v2,
            const ShaderDataList& v3,
            float alpha, float beta, float gamma
            );
protected:
};

} // namespace components

#endif //SEELE_TRI_MESH_H