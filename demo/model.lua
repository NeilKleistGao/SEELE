--[[--
 - Copyright (C) 2007 Free Software Foundation, Inc.
 - https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 - The GNU General Public License is a free, copyleft license for software and other kinds of works.
 - The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 - By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 - We, the Free Software Foundation, use the GNU General Public License for most of our software;
 - it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 --]]

--- @file model.lua

function onLoad()
    model = seele.Model("demo/obj/african_head.obj", "demo/texture/african_head_diffuse.tga")
    model:setScale(seele.Vector(3, 3, 3))
    --model:setPosition(seele.Vector(0, 0, -1))
    --model:setRotation(seele.Vector(3.14 / 8, 3.14 / 8, 3.14 / 16))
end

function onUpdate(delta)
    Renderer.clearWithColor(0, 0, 0)
    model:draw()
end