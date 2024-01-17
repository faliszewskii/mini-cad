#include <iostream>
#include "glad/glad.h"
#include "OpenGLInstance.h"
#include "imgui.h"

/**
 * Function to initialize GLFW, Glad, create a GLFWwindow instance and enable OpenGL functions.
 * @param screenWidth
 * @param screenHeight
 * @param mouseCallback
 * @param scrollCallback
 * @return Success status.
 */
int OpenGLInstance::init(
        int screenWidth, int screenHeight
) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Sandbox", 0, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    auto framebuffer_size_callback = [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    };

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Glad initialization
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enables section
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

bool OpenGLInstance::isRunning() {
    return !glfwWindowShouldClose(window);
}

GLFWwindow *OpenGLInstance::getWindow() {
    return window;
}

void OpenGLInstance::swapBuffers() {
    glfwSwapBuffers(window);
}

void OpenGLInstance::pollEvents() {
    glfwPollEvents();
}

