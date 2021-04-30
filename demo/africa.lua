--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- africa.lua

SHADERS_LIST = {
    "vertex", "fragment"
}

function onCreate()
    head = seele.TriMesh("../demo/assets/Africa/african_head.obj", "vertex", "fragment")
    head:setRotation(seele.vec3(0, 180, 0))
    head:setScale(seele.vec3(300, 300, 300))
    head:setPosition(seele.vec3(0, 0, -400))
    R:addObject(head)

    camera = seele.Camera(
            seele.vec3(0, 0, 0),
            seele.vec3(0, 0, -1),
            seele.vec3(0, -1, 0),
            90, 1024, 768
    )
    R:setCamera(camera)
end

function vertex(app_data)
    position = R:transformMVP(app_data[1])
    return {
        {3, position, true},
        {2, app_data[2], true},
        {3, app_data[3], true}
    }
end

function fragment(v2f)
    color = R:getTextureColor(0, v2f[2].x, v2f[2].y)
    return seele.vec3(color.x, color.y, color.z)
end