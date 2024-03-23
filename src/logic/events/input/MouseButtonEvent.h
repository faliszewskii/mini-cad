//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_MOUSEBUTTONEVENT_H
#define OPENGL_SANDBOX_MOUSEBUTTONEVENT_H

#include <GLFW/glfw3.h>

struct MouseButtonEvent {
    GLFWwindow *window;
    int button;
    int action;
    int mods;
};

#endif //OPENGL_SANDBOX_MOUSEBUTTONEVENT_H
