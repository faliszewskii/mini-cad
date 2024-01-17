//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_OPENGLINSTANCE_H
#define OPENGL_SANDBOX_OPENGLINSTANCE_H

#include <memory>
#include "../../lib/glad/glad_glfw.h"
#include "../ApplicationState.h"

class OpenGLInstance {
    GLFWwindow *window;
public:
    int init(int screenWidth, int screenHeight);

    bool isRunning();

    GLFWwindow *getWindow();

    void swapBuffers();

    static void pollEvents();
};


#endif //OPENGL_SANDBOX_OPENGLINSTANCE_H
