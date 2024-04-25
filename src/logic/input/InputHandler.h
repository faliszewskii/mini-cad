//
// Created by faliszewskii on 10.01.24.
//

#ifndef OPENGL_SANDBOX_INPUTHANDLER_H
#define OPENGL_SANDBOX_INPUTHANDLER_H


#include "../../../lib/glad/glad_glfw.h"
#include <functional>
#include "../state/AppState.h"

class InputHandler {
    AppState &appState;
public:
    explicit InputHandler(std::unique_ptr<AppState> &applicationState);

    void setupCallbacks(GLFWwindow *window);
};


#endif //OPENGL_SANDBOX_INPUTHANDLER_H
