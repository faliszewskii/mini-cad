//
// Created by faliszewskii on 10.01.24.
//

#include "InputHandler.h"
#include "imgui.h"

InputHandler::InputHandler(std::unique_ptr<ApplicationState> &applicationState) :
        inputHandlerState(applicationState->currentCamera, true) {}

void InputHandler::setupCallbacks(GLFWwindow *window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->keyCallback(window, key, scancode, action, mods);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->mouseCallback(window, xpos, ypos);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->scrollCallback(window, xoffset, yoffset);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->mouseButtonCallback(window, button, action, mods);
    });
}


void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) const {
    static auto deltaTime = static_cast<float>(glfwGetTime());
    deltaTime = static_cast<float>(glfwGetTime()) - deltaTime;
    if (!inputHandlerState.currentCamera) return;

    if (ImGui::GetIO().WantCaptureKeyboard) return;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        inputHandlerState.currentCamera.value().get().processKeyboard(FORWARD, deltaTime);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        inputHandlerState.currentCamera.value().get().processKeyboard(BACKWARD, deltaTime);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        inputHandlerState.currentCamera.value().get().processKeyboard(LEFT, deltaTime);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        inputHandlerState.currentCamera.value().get().processKeyboard(RIGHT, deltaTime);
}

void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        inputHandlerState.guiFocus = false;
    else
        inputHandlerState.guiFocus = true;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void InputHandler::mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    static float lastX = xpos;
    static float lastY = ypos;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (inputHandlerState.currentCamera && !inputHandlerState.guiFocus) {
        inputHandlerState.currentCamera.value().get().processMouseMovement(xoffset, yoffset);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (!inputHandlerState.currentCamera || ImGui::GetIO().WantCaptureMouse) return;
    inputHandlerState.currentCamera.value().get().processMouseScroll(static_cast<float>(yoffset));
}
