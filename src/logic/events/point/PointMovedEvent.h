//
// Created by faliszewskii on 10.03.24.
//

#ifndef OPENGL_SANDBOX_POINTMOVEDEVENT_H
#define OPENGL_SANDBOX_POINTMOVEDEVENT_H

#include "../../geometry/Point.h"

struct PointMovedEvent {
    Point &point;
    glm::vec3 delta;
};

#endif //OPENGL_SANDBOX_POINTMOVEDEVENT_H
