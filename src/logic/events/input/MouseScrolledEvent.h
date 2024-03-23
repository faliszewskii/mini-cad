//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_MOUSESCROLLEDEVENT_H
#define OPENGL_SANDBOX_MOUSESCROLLEDEVENT_H

#include <GLFW/glfw3.h>

struct MouseScrolledEvent {
    GLFWwindow *window;
    double xoffset;
    double yoffset;
};

#endif //OPENGL_SANDBOX_MOUSESCROLLEDEVENT_H
