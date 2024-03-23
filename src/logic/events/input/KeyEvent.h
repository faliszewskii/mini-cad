//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_KEYEVENT_H
#define OPENGL_SANDBOX_KEYEVENT_H

#include <GLFW/glfw3.h>

struct KeyEvent {
    GLFWwindow *window;
    int key;
    int scancode;
    int action;
    int mods;
};

#endif //OPENGL_SANDBOX_KEYEVENT_H
