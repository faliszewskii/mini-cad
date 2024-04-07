//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_INPUTEVENTSHANDLER_H
#define OPENGL_SANDBOX_INPUTEVENTSHANDLER_H

#include "../../state/AppState.h"

namespace InputEventsHandler {

    void clickPoint(AppState &appState, double xpos, double ypos, const glm::mat4 &m, float epsilon, Point &point);

    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;

        /// Mouse Button Events
        eventPublisher.subscribe([&](const MouseButtonEvent &event){
            if (event.button == GLFW_MOUSE_BUTTON_RIGHT && event.action == GLFW_PRESS)
                appState.guiFocus = false;
            else {
                appState.guiFocus = true;
                double xpos, ypos;
                auto &io = ImGui::GetIO();
                glfwGetCursorPos(event.window, &xpos, &ypos);
                xpos = (xpos - appState.guiPanelLeftWidth) / (io.DisplaySize.x - appState.guiPanelLeftWidth);
                xpos = xpos * 2 - 1;
                ypos = ypos / io.DisplaySize.y;
                ypos = -2 * ypos + 1;
                glm::mat4 view = appState.camera.getViewMatrix();
                glm::mat4 projection = appState.camera.getProjectionMatrix();
                if(event.button == GLFW_MOUSE_BUTTON_LEFT && event.action == GLFW_PRESS) {
                    glm::mat4 m = projection * view;
                    float epsilon = 0.01;
                    for(auto &point : std::views::values(appState.pointSet)) { // Normal points click
                        clickPoint(appState, xpos, ypos, m, epsilon, *point);
                    }
                    for(auto &bezierC2 : std::views::values(appState.bezierC2Set)) {
                        if(!bezierC2->drawBernstein) continue;
                        for(auto &point : bezierC2->bernsteinPoints) {
                            clickPoint(appState, xpos, ypos, m, epsilon, *point);
                        }
                    }
                }
                if(event.button == GLFW_MOUSE_BUTTON_MIDDLE && event.action == GLFW_PRESS) {
                    glm::vec4 world = glm::inverse(projection * view) * glm::vec4(xpos, ypos, 0.995f, 1.f);
                    world /= world.w;
                    appState.cursorPosition = world;
                }
            }
        });

        /// Mouse Moved Events
        eventPublisher.subscribe([&](const MouseMovedEvent &event){
            auto xpos = static_cast<float>(event.xPosIn);
            auto ypos = static_cast<float>(event.yPosIn);

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
                glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        });

        /// Mouse Scrolled Events
        eventPublisher.subscribe([&](const MouseScrolledEvent &event){
            if (ImGui::GetIO().WantCaptureMouse) return;
            appState.camera.processMouseScroll(static_cast<float>(event.yoffset));
        });

        /// Key Events
        eventPublisher.subscribe([&](const KeyEvent &event){
            static auto deltaTime = static_cast<float>(glfwGetTime());
            deltaTime = static_cast<float>(glfwGetTime()) - deltaTime;

            if (ImGui::GetIO().WantCaptureKeyboard) return;
            if (event.key == GLFW_KEY_ESCAPE && event.action == GLFW_PRESS)
                glfwSetWindowShouldClose(event.window, true);
            if (event.key == GLFW_KEY_W && event.action == GLFW_PRESS)
                appState.camera.processKeyboard(FORWARD, deltaTime);
            if (event.key == GLFW_KEY_S && event.action == GLFW_PRESS)
                appState.camera.processKeyboard(BACKWARD, deltaTime);
            if (event.key == GLFW_KEY_A && event.action == GLFW_PRESS)
                appState.camera.processKeyboard(LEFT, deltaTime);
            if (event.key == GLFW_KEY_D && event.action == GLFW_PRESS)
                appState.camera.processKeyboard(RIGHT, deltaTime);
            if (event.key == GLFW_KEY_LEFT_CONTROL) {
                if(event.action == GLFW_PRESS)
                    appState.keyboardCtrlMode = true;
                else if(event.action == GLFW_RELEASE)
                    appState.keyboardCtrlMode = false;
            }
        });
    }

    void clickPoint(AppState &appState, double xpos, double ypos, const glm::mat4 &m, float epsilon, Point &point) {
        glm::vec4 coords = m * glm::vec4(point.position, 1);
        coords /= coords.w;
        if(std::abs(coords.x - xpos) < epsilon && std::abs(coords.y - ypos) < epsilon)
            appState.eventPublisher.publish(SelectEntityEvent{point, 0});
    }
}

#endif //OPENGL_SANDBOX_INPUTEVENTSHANDLER_H
