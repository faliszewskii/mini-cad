//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_OPENGLINSTANCE_H
#define OPENGL_SANDBOX_OPENGLINSTANCE_H


#include <GLFW/glfw3.h>

class OpenGLInstance {
    GLFWwindow* window;
public:
    int init(int screenWidth, int screenHeight, void (*mouseCallback)(GLFWwindow *, double, double), void (*mouseButtonCallback)(GLFWwindow* , int , int, int), void (*scrollCallback)(GLFWwindow *, double, double));

    bool isRunning();

    GLFWwindow *getWindow();

    void swapBuffers();

    void pollEvents();
};


#endif //OPENGL_SANDBOX_OPENGLINSTANCE_H
