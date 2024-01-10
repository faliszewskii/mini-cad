
#include <memory>

#include "../../lib/glad/glad_glfw.h"
#define STB_IMAGE_IMPLEMENTATION
#include "logic/io/stb_image.h"

#include "presentation/rendering/shader/shader.h"
#include "logic/opengl/OpenGLInstance.h"
#include "logic/io/IOUtils.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "logic/generator/ModelGenerator.h"
#include "presentation/gui/GUI.h"
#include "logic/ApplicationState.h"
#include "presentation/rendering/renderVisitor/RenderSceneVisitor.h"
#include "presentation/scene/nodes/camera/Camera.h"
#include "logic/input/InputHandler.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    OpenGLInstance openGlInstance{};
    openGlInstance.init(SCR_WIDTH, SCR_HEIGHT);

    // TODO Do research on paths in C++. Try to be as OS agnostic as possible. Cerberus model had the 'windows slash' problem
    Shader albedoShader("albedo", IOUtils::getResource("shaders/basic/albedo.vert"), IOUtils::getResource("shaders/basic/albedo.frag"));
    Shader monoShader("mono", IOUtils::getResource("shaders/basic/mono.vert"), IOUtils::getResource("shaders/basic/mono.frag"));
    monoShader.use(); // TODO Init somewhere else
    monoShader.setVec4("color", glm::vec4(1, 0, 1, 0.5f));
    Shader whiteShader("white", IOUtils::getResource("shaders/basic/white.vert"), IOUtils::getResource("shaders/basic/white.frag"));
    std::vector<Shader> defaultShaders{albedoShader, monoShader, whiteShader};
    std::unique_ptr<ApplicationState> appState = std::make_unique<ApplicationState>(defaultShaders);
    InputHandler inputHandler(appState);
    inputHandler.setupCallbacks(openGlInstance.getWindow());

    appState->rootSceneNode.addChild(ModelGenerator::generateAxis());

    GUI gui(openGlInstance.getWindow(), *appState);

    while (openGlInstance.isRunning())
    {
        OpenGLInstance::pollEvents();
        gui.newFrame();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // activate shader
        appState->globalShader.get().use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // TODO configurable
        appState->globalShader.get().setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = appState->currentCamera.get().getViewMatrix();
        appState->globalShader.get().setMat4("view", view);

        RenderSceneVisitor renderSceneVisitor(appState->globalShader);
        appState->rootSceneNode.visitTree(renderSceneVisitor);

        gui.render();

        openGlInstance.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
