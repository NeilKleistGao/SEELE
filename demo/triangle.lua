--[[--
 - Copyright (C) 2007 Free Software Foundation, Inc.
 - https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 - The GNU General Public License is a free, copyleft license for software and other kinds of works.
 - The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 - By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 - We, the Free Software Foundation, use the GNU General Public License for most of our software;
 - it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 --]]

--- @file triangle.lua


function onLoad()
end

function onUpdate(delta)
    Renderer.clearWithColor(0, 0, 0)
    Renderer.setColor(0x66, 0xcc, 0xff, 0xff)

    Renderer.triangle(seele.Vector(100, 100, 0), seele.Vector(100, 200, 0), seele.Vector(200, 100, 0))
    Renderer.triangle(seele.Vector(300, 300, 0), seele.Vector(400, 300, 0), seele.Vector(350, 500, 0))
    Renderer.triangle(seele.Vector(300, 700, 0), seele.Vector(100, 600, 0), seele.Vector(600, 600, 0))
end