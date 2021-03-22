--[[--
 - Copyright (C) 2007 Free Software Foundation, Inc.
 - https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 - The GNU General Public License is a free, copyleft license for software and other kinds of works.
 - The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 - By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 - We, the Free Software Foundation, use the GNU General Public License for most of our software;
 - it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 --]]

--- @file z_buffer.lua

function onLoad()
end

function onUpdate(delta)
    Renderer.clearWithColor(0, 0, 0)

    Renderer.setColor(255, 0, 0, 255)
    Renderer.line(seele.Vector(100, 100, 5), seele.Vector(200, 200, 5))
    Renderer.line(seele.Vector(101, 100, 5), seele.Vector(200, 199, 5))
    Renderer.line(seele.Vector(100, 101, 5), seele.Vector(199, 200, 5))
    Renderer.setColor(0, 0, 255, 255)
    Renderer.line(seele.Vector(200, 100, 0), seele.Vector(100, 200, 0))
    Renderer.line(seele.Vector(199, 100, 0), seele.Vector(100, 199, 0))
    Renderer.line(seele.Vector(200, 101, 0), seele.Vector(101, 200, 0))

    Renderer.setColor(255, 0, 0, 255)
    Renderer.triangle(seele.Vector(300, 300, 2), seele.Vector(400, 300, 2), seele.Vector(350, 400, 2))
    Renderer.setColor(0, 0, 255, 255)
    Renderer.line(seele.Vector(350, 250, 0), seele.Vector(350, 450, 4))

    Renderer.setColor(255, 0, 0, 255)
    Renderer.triangle(seele.Vector(600, 300, 1), seele.Vector(700, 300, 1), seele.Vector(650, 400, 5))
    Renderer.setColor(0, 0, 255, 255)
    Renderer.triangle(seele.Vector(600, 400, 2), seele.Vector(700, 400, 2), seele.Vector(650, 200, 2))
end