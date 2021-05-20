--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- sphere.lua

SHADERS_LIST = {
    "vertex", "fragment"
}

light = nil

function onCreate()
    sphere = seele.Sphere(100, {"vertex"}, {"fragment"})
    sphere:setPosition(seele.vec3(0, 0, 0))
    R:addObject(sphere)

    camera = seele.Camera(
            seele.vec3(0, -110, 0),
            seele.vec3(0, 1, 0),
            seele.vec3(0, 0, 1),
            15, 400, 225
    )
    R:setCamera(camera)

    light = seele.DirectionalLight(seele.normalize(seele.vec3(0, 1, 10)), seele.vec3(255, 255, 255))
end

function vertex(app_data)
    position = R:transformMVP(app_data[1])
    normal = R:transformNormal(app_data[3])
    return {
        {3, position, false},
        {2, app_data[2], false},
        {3, normal, false}
    }
end

function clamp(color)
    return seele.vec3(math.min(color.x, 255),
            math.min(color.y, 255),
            math.min(color.z, 255))
end

function dot(v1, v2)
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
end

function getReflect(normal, light)
    cof = dot(normal, light) * 2
    return seele.vec3(normal.x * cof - light.x, normal.y * cof - light.y, normal.z * cof - light.z)
end

function fragment(v2f)
    color = seele.vec3(0x66, 0xcc, 0xff)
    normal = v2f[3]
    res = seele.vec3(color.x * 0.3, color.y * 0.3, color.z * 0.3)
    --res = seele.vec3(0, 0, 0)

    pos = v2f[1]
    data = light:getLightData(v2f[1])
    theta = dot(normal, data.direction)
    if theta > 0.0 then
        res.x = res.x + color.x * theta
        res.y = res.y + color.y * theta
        res.z = res.z + color.z * theta
    end

    reflect = getReflect(normal, data.direction)
    spec_coff = 0.0001^(math.max(0, reflect.z))
    res.x = res.x + 255 * spec_coff * 0.01
    res.y = res.y + 255 * spec_coff * 0.01
    res.z = res.z + 255 * spec_coff * 0.01

    return clamp(res)
end