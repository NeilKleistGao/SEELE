--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- rt_sphere.lua

SHADERS_LIST = {
    "lambertian"
}

albedo = seele.vec3(255, 255, 255)

function onCreate()
    math.randomseed(tostring(os.time()))

    sphere = seele.Sphere(0.5, nil, {"lambertian"})
    sphere:setPosition(seele.vec3(0, 0, -1))
    R:addObject(sphere)

    terrain = seele.Sphere(100, nil, {"lambertian"})
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

function lambertian(ray, record)
    local scatter_direction = record.normal
    local random_direction = getRandomDirectionInUnitSphere()
    scatter_direction.x = scatter_direction.x + random_direction.x
    scatter_direction.y = scatter_direction.y + random_direction.y
    scatter_direction.z = scatter_direction.z + random_direction.z

    local scatter_ray = seele.Ray(record.position, scatter_direction)
    return {true, albedo, scatter_ray}
end

function getRandomDirectionInUnitSphere()
    while (true)
    do
        local dir = seele.vec3(math.random(-1, 1), math.random(-1, 1), math.random(-1, 1))
        local dot = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z
        if dot < 1 then
            return dir
        end
    end
end