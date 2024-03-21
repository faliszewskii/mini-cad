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

    if (ImGui::GetIO().WantCaptureKeyboard) return;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        appState.camera.processKeyboard(FORWARD, deltaTime);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        appState.camera.processKeyboard(BACKWARD, deltaTime);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        appState.camera.processKeyboard(LEFT, deltaTime);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        appState.camera.processKeyboard(RIGHT, deltaTime);
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if(action == GLFW_PRESS)
            appState.keyboardCtrlMode = true;
        else if(action == GLFW_RELEASE)
            appState.keyboardCtrlMode = false;
    }
}

void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        appState.guiFocus = false;
    else {
        appState.guiFocus = true;
        double xpos, ypos;
        auto &io = ImGui::GetIO();
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos = (xpos - appState.guiPanelLeftWidth) / (io.DisplaySize.x - appState.guiPanelLeftWidth);
        xpos = xpos * 2 - 1;
        ypos = ypos / io.DisplaySize.y;
        ypos = -2 * ypos + 1;
        glm::mat4 view = appState.camera.getViewMatrix();
        glm::mat4 projection = appState.camera.getProjectionMatrix();
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            glm::mat4 m = projection * view;
            float epsilon = 0.01;
            for(auto &point : std::views::values(appState.pointSet)) { // TODO Maybe not on every click.
                glm::vec4 coords = m * glm::vec4(point->position, 1);
                coords /= coords.w;
                if(std::abs(coords.x - xpos) < epsilon && std::abs(coords.y - ypos) < epsilon)
                    appState.eventPublisher.publish(SelectEntityEvent{*point});
            }
        }
        if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
            glm::vec4 world = glm::inverse(projection * view) * glm::vec4(xpos, ypos, 0.995f, 1.f);
            world /= world.w;
            appState.cursorPosition = world;
        }
    }
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

    if (!appState.guiFocus) {
        appState.camera.processMouseMovement(xoffset, yoffset);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    appState.camera.processMouseScroll(static_cast<float>(yoffset));
}
