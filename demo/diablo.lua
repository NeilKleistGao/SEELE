--- Copyright (C) 2007 Free Software Foundation, Inc.
--- https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
--- The GNU General Public License is a free, copyleft license for software and other kinds of works.
--- The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
--- By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
--- We, the Free Software Foundation, use the GNU General Public License for most of our software;
--- it applies also to any other work released this way by its authors. You can apply it to your programs, too.

--- diablo.lua

SHADERS_LIST = {
    "vertex", "fragment", "depthVertex", "depthFragment"
}

light = nil
light_camera = nil
camera = nil
depth_buffer = {}

EPSILON = 16.1145141919810

function onCreate()
    head = seele.TriMesh("../demo/assets/Diablo/diablo3_pose.obj",
            {"depthVertex", "vertex"}, {"depthFragment", "fragment"})
    head:setRotation(seele.vec3(0, 0, 0))
    head:setScale(seele.vec3(300, 300, 300))
    head:setPosition(seele.vec3(0, 0, 0))
    R:addObject(head)

    light_camera = seele.Camera(
            seele.vec3(0, -1000, 0),
            seele.vec3(0, 1, 0),
            seele.vec3(0, 0, 1),
            90, 1024, 768
    )
    R:setCamera(light_camera)
    R:setPass(2)

    light = seele.DirectionalLight(seele.normalize(seele.vec3(0, 1, 0)), seele.vec3(255, 255, 255))

    for i = 0, 1023 do
        for j = 0, 767 do
            depth_buffer[j * 1024 + i] = -100000
        end
    end
end

function changePass(former, later)
    camera = seele.Camera(
            seele.vec3(0, 0, -400),
            seele.vec3(0, 0, 1),
            seele.vec3(0, -1, 0),
            90, 1024, 768
    )
    R:setCamera(camera)
end

function vertex(app_data)
    position = R:transformMVP(app_data[1])
    normal = R:transformNormal(app_data[3])

    R:setCamera(light_camera)
    light_position = R:transformMVP(app_data[1])
    R:setCamera(camera)

    return {
        {3, position, true},
        {2, app_data[2], true},
        {3, normal, true},
        {3, light_position, true}
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
    ambient_color = R:getTextureColor(0, v2f[2].x, v2f[2].y)
    normal = v2f[3]
    light_pos = v2f[4]
    material = R:getMaterial()
    res = seele.vec3(ambient_color.x * material.Ka.x, ambient_color.y * material.Ka.y, ambient_color.z * material.Ka.z)

    diffuse_color = R:getTextureColor(1, v2f[2].x, v2f[2].y)
    data = light:getLightData(v2f[1])
    theta1 = dot(normal, data.direction)
    if theta1 >= 0.0 then
        res.x = res.x + diffuse_color.x * material.Kd.x * theta1
        res.y = res.y + diffuse_color.y * material.Kd.y * theta1
        res.z = res.z + diffuse_color.z * material.Kd.z * theta1
    end

    spec_coff = R:getTextureColor(2, v2f[2].x, v2f[2].y).x
    reflect = getReflect(normal, data.direction)
    spec_coff = spec_coff^(math.max(0, reflect.z))
    res.x = res.x + diffuse_color.x * material.Ks.x * spec_coff
    res.y = res.y + diffuse_color.y * material.Ks.y * spec_coff
    res.z = res.z + diffuse_color.z * material.Ks.z * spec_coff

    lx = math.ceil(light_pos.x + 0.5) + 512
    ly = math.ceil(light_pos.y + 0.5) + 384
    if lx > -1 and lx < 1024 and ly > -1 and ly < 768 and depth_buffer[ly * 1024 + lx] > light_pos.z + EPSILON then
        res.x = res.x * 0.5
        res.y = res.y * 0.5
        res.z = res.z * 0.5
    end

    return clamp(res)
end

function depthVertex(app_data)
    position = R:transformMVP(app_data[1])
    return {
        {3, position, true}
    }
end

function depthFragment(v2f)
    pos = v2f[1]
    z = pos.z
    x = math.ceil(pos.x + 0.5) + 512
    y = math.ceil(pos.y + 0.5) + 384
    z = (z + 300) / 600

    if x > -1 and x < 1024 and y > -1 and y < 768 and depth_buffer[y * 1024 + x] < pos.z - EPSILON then
        depth_buffer[y * 1024 + x] = pos.z
    end

    return seele.vec3(-1, -1, -1)
    --return seele.vec3(255 * z, 255 * z, 255 * z)
end