--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- glass.lua

SHADERS_LIST = {
    "sphereScatter", "terrainScatter", "metalScatter", "dielectric"
}

random = nil

function onCreate()
    random = seele.Random()

    sphere0 = seele.Sphere(0.5, nil, {"dielectric"})
    sphere0:setPosition(seele.vec3(-1, 0, -1))
    R:addObject(sphere0)
    sphere1 = seele.Sphere(0.5, nil, {"sphereScatter"})
    sphere1:setPosition(seele.vec3(0, 0, -1))
    R:addObject(sphere1)
    sphere2 = seele.Sphere(0.5, nil, {"metalScatter"})
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

function refract(v, n, eta_ratio)
    local nv = seele.vec3(-v.x, -v.y, -v.z)
    local cos_theta = dot(nv, n)
    local num_sin = math.sqrt(1 - cos_theta * cos_theta)
    local sin_theta = seele.vec3(v.x + cos_theta * n.x, v.y + cos_theta * n.y, v.z + cos_theta * n.z)
    if eta_ratio * num_sin > 1 then
        return reflect(v, n)
    end

    local perpendicular = seele.vec3(eta_ratio * sin_theta.x, eta_ratio * sin_theta.y, eta_ratio * sin_theta.z)
    local len = -math.sqrt(1 - dot(perpendicular, perpendicular))
    local parallel = seele.vec3(len * n.x, len * n.y, len * n.z)

    return seele.vec3(perpendicular.x + parallel.x, perpendicular.y + parallel.y, perpendicular.z + parallel.z)
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
    return lambertian(ray, record, seele.vec3(25, 50, 128))
end

function terrainScatter(ray, record)
    return lambertian(ray, record, seele.vec3(204, 204, 0))
end

function metalScatter(ray, record)
    return metal(ray, record, seele.vec3(204, 153, 51))
end

function dielectric(ray, record)
    local ir = 1.5
    local ratio = ((record.front) and {1.0 / ir} or {ir})[1]
    local refract_dir = refract(ray:getDirection(), record.normal, ratio)
    local scatter_ray = seele.Ray(record.position, refract_dir)

    return {true, seele.vec3(255, 255, 255), scatter_ray}
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