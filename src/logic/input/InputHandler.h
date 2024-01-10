//
// Created by faliszewskii on 10.01.24.
//

#ifndef OPENGL_SANDBOX_INPUTHANDLER_H
#define OPENGL_SANDBOX_INPUTHANDLER_H


#include "../../../lib/glad/glad_glfw.h"
#include <functional>
#include "../../presentation/scene/nodes/camera/Camera.h"
#include "../ApplicationState.h"

struct InputHandlerState {
    std::reference_wrapper<Camera> &currentCamera;
    bool guiFocus;
};

class InputHandler {
    InputHandlerState inputHandlerState;
public:
    explicit InputHandler(std::unique_ptr<ApplicationState>& applicationState);

    void setupCallbacks(GLFWwindow* window);
private:
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) const;
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);
    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};


#endif //OPENGL_SANDBOX_INPUTHANDLER_H
