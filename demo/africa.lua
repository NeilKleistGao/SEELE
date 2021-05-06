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

light = nil

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

    light = seele.DirectionalLight(seele.vec3(0, 0, -1), seele.vec3(255, 255, 255))
end

function vertex(app_data)
    position = R:transformMVP(app_data[1])
    normal = R:transformNormal(app_data[3])
    return {
        {3, position, true},
        {2, app_data[2], true},
        {3, normal, true}
    }
end

function clamp(color)
    return seele.vec3(color.x > 255 and 255 or color.x,
            color.y > 255 and 255 or color.y,
            color.z > 255 and 255 or color.y)
end

function dot(v1, v2)
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
end

function fragment(v2f)
    color = R:getTextureColor(0, v2f[2].x, v2f[2].y)
    normal = v2f[3]
    material = R:getMaterial()
    res = seele.vec3(color.x * material.Ka.x, color.y * material.Ka.y, color.z * material.Ka.z)

    data = light:getLightData(v2f[1])
    theta1 = dot(normal, data.direction)
    if theta1 >= 0.0 then
        res.x = res.x + color.x * material.Kd.x * theta1
        res.y = res.y + color.y * material.Kd.y * theta1
        res.z = res.z + color.z * material.Kd.z * theta1
    end

    return clamp(res)
end