//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_MOUSEMOVEDEVENT_H
#define OPENGL_SANDBOX_MOUSEMOVEDEVENT_H

#include <GLFW/glfw3.h>

struct MouseMovedEvent {
    GLFWwindow *window;
    double xPosIn;
    double yPosIn;
};

#endif //OPENGL_SANDBOX_MOUSEMOVEDEVENT_H
