--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- rt_sphere.lua

SHADERS_LIST = {
    "vertex", "fragment", "vertex2", "fragment2"
}

function onCreate()
    sphere = seele.Sphere(0.5, {"vertex"}, {"fragment"})
    sphere:setPosition(seele.vec3(0, 0, -1))
    R:addObject(sphere)

    terrain = seele.Sphere(100, {"vertex2"}, {"fragment2"})
    terrain:setPosition(seele.vec3(0, -100.5, -1))
    R:addObject(terrain)

    camera = seele.Camera(
            seele.vec3(0, 0, 0),
            seele.vec3(0, 0, -1),
            seele.vec3(0, 1, 0),
            90, 400, 225
    )
    R:setCamera(camera)
end

function vertex(app_data)
    return {}
end

function fragment(v2f)
    return seele.vec3(255, 0, 0)
end

function vertex2(app_data)
    return {}
end

function fragment2(v2f)
    return seele.vec3(0, 255, 0)
end