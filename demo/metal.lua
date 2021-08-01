--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- metal.lua

SHADERS_LIST = {
    "sphereScatter", "terrainScatter", "metalScatter1", "metalScatter2"
}

random = nil

function onCreate()
    random = seele.Random()

    sphere0 = seele.Sphere(0.5, nil, {"metalScatter1"})
    sphere0:setPosition(seele.vec3(-1, 0, -1))
    R:addObject(sphere0)
    sphere1 = seele.Sphere(0.5, nil, {"sphereScatter"})
    sphere1:setPosition(seele.vec3(0, 0, -1))
    R:addObject(sphere1)
    sphere2 = seele.Sphere(0.5, nil, {"metalScatter2"})
    sphere2:setPosition(seele.vec3(1, 0, -1))
    R:addObject(sphere2)

    terrain = seele.Sphere(100, nil, {"terrainScatter"})
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

function dot(v1, v2)
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
end

function reflect(vec, normal)
    local d = 2 * dot(vec, normal)
    return seele.vec3(vec.x - normal.x * d, vec.y - normal.y * d, vec.z - normal.z * d)
end

function metal(ray, record, albedo)
    local in_direction = ray:getDirection()
    local scatter_direction = reflect(in_direction, record.normal)
    local scatter_ray = seele.Ray(record.position, scatter_direction)

    return {dot(scatter_direction, record.normal) > 0, albedo, scatter_ray}
end

function lambertian(ray, record, albedo)
    local scatter_direction = record.normal
    local random_direction = getRandomDirectionInUnitSphere()
    scatter_direction.x = scatter_direction.x + random_direction.x
    scatter_direction.y = scatter_direction.y + random_direction.y
    scatter_direction.z = scatter_direction.z + random_direction.z

    local scatter_ray = seele.Ray(record.position, scatter_direction)

    return {true, albedo, scatter_ray}
end

function sphereScatter(ray, record)
    return lambertian(ray, record, seele.vec3(178, 76, 76))
end

function terrainScatter(ray, record)
    return lambertian(ray, record, seele.vec3(204, 204, 0))
end

function metalScatter1(ray, record)
    return metal(ray, record, seele.vec3(204, 204, 204))
end

function metalScatter2(ray, record)
    return metal(ray, record, seele.vec3(204, 153, 51))
end

function getRandomDirectionInUnitSphere()
    while (true)
    do
        local dir = seele.vec3(random:roll(-1, 1), random:roll(-1, 1), random:roll(-1, 1))
        local len2 = dot(dir, dir)
        if len2 < 1 then
            return dir
        end
    end
end