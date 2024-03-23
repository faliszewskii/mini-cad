//
// Created by faliszewskii on 10.01.24.
//

#include "InputHandler.h"
#include "imgui.h"

InputHandler::InputHandler(std::unique_ptr<AppState> &applicationState) :
        appState(*applicationState) {}

void InputHandler::setupCallbacks(GLFWwindow *window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->appState.eventPublisher.publish(KeyEvent{ window, key, scancode, action, mods});
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->appState.eventPublisher.publish(MouseMovedEvent{window, xpos, ypos});
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->appState.eventPublisher.publish(MouseScrolledEvent{window, xoffset, yoffset});
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->appState.eventPublisher.publish(MouseButtonEvent{window, button, action, mods});
    });
}
