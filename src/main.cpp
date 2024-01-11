
#include <memory>

#include "../../lib/glad/glad_glfw.h"
#define STB_IMAGE_IMPLEMENTATION
#include "logic/io/stb_image.h"

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

    auto appState = std::make_unique<ApplicationState>();

    InputHandler inputHandler(appState);
    inputHandler.setupCallbacks(openGlInstance.getWindow());

    {
        GUI gui(openGlInstance.getWindow(), *appState);

        // TODO Do research on paths in C++. Try to be as OS agnostic as possible. Cerberus model had the 'windows slash' problem
        auto shader = std::make_unique<Shader>("albedo", IOUtils::getResource("shaders/basic/albedo.vert"), IOUtils::getResource("shaders/basic/albedo.frag"));
        appState->rootSceneNode.addChild(std::make_unique<SceneTreeNode>(std::move(shader)));

        auto camera = std::make_unique<Camera>("camera", SCR_WIDTH-gui.getGuiWidth(), SCR_HEIGHT, CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f)); // TODO Set orientation to anchor
        appState->currentCamera = *camera.get();
        appState->rootSceneNode.addChild(std::make_unique<SceneTreeNode>(std::move(camera)));

        appState->rootSceneNode.addChild(ModelGenerator::generateAxis());

        while (openGlInstance.isRunning()) {
            OpenGLInstance::pollEvents();
            gui.newFrame();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(gui.getGuiWidth(), 0, SCR_WIDTH-gui.getGuiWidth(), SCR_HEIGHT);
            RenderSceneVisitor renderSceneVisitor{};
            appState->rootSceneNode.visitTree(renderSceneVisitor);

            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            gui.render();

            openGlInstance.swapBuffers();
        }
    }

    glfwTerminate();
    return 0;
}
