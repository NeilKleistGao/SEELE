/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file camera.h

#ifndef SEELE_CAMERA_H
#define SEELE_CAMERA_H

#include "glm/glm.hpp"

namespace rendering {

class Camera {
public:
    static Camera* getInstance();
    static void destroyInstance();

    void init(const glm::vec3& position, const glm::vec3& look_at, const glm::vec4& perspective);

    glm::vec4 viewPointTransform(const glm::vec4& vec);
    glm::vec4 perspectiveTransform(const glm::vec4& vec);
private:
    static Camera* _instance;

    glm::mat4 _view_point;
    glm::mat4 _perspective;
protected:
    Camera();
    ~Camera();
};

} // namespace rendering

#endif //SEELE_CAMERA_H
