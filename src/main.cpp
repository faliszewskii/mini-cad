
#include <memory>

#define STB_IMAGE_IMPLEMENTATION

#include "logic/io/stb_image.h"
#include "../../lib/glad/glad_glfw.h"
#include "logic/opengl/OpenGLInstance.h"
#include "logic/io/IOUtils.h"
#include "logic/generator/ModelGenerator.h"
#include "presentation/gui/GUI.h"
#include "logic/AppState.h"
#include "presentation/scene/nodes/camera/Camera.h"
#include "presentation/scene/nodes/light/Light.h"
#include "logic/input/InputHandler.h"
#include "presentation/scene/nodes/frameBuffer/FrameBuffer.h"
#include "presentation/rendering/renderVisitor/ExecuteStepVisitor.h"

// settings
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

int main() {
    OpenGLInstance openGlInstance{};
    openGlInstance.init(SCR_WIDTH, SCR_HEIGHT);

    auto appState = std::make_unique<AppState>();

    InputHandler inputHandler(appState);
    inputHandler.setupCallbacks(openGlInstance.getWindow());

    {
        GUI gui(openGlInstance.getWindow(), *appState);

        auto &frameBuffer = appState->scene.addSceneNode(FrameBuffer("Main"));
        appState->scene.addStep(AddFrameBuffer(frameBuffer));

//         TODO Do research on paths in C++. Try to be as OS agnostic as possible. Cerberus model had the 'windows slash' problem
        auto &randomShader = appState->scene.addSceneNode(Shader("random",
                                     IOUtils::getResource("shaders/basic/white.vert"),
                                     IOUtils::getResource("shaders/debug/random.frag")));
        randomShader.setActive(false);
        appState->scene.addStep(ActivateShader(randomShader));

        auto &normalShader = appState->scene.addSceneNode(Shader("normalDebug",
                                     IOUtils::getResource("shaders/debug/normalDebug.vert"),
                                     IOUtils::getResource("shaders/debug/normalDebug.geom"),
                                     IOUtils::getResource("shaders/debug/normalDebug.frag")));
        normalShader.setActive(false);
//        randomShader->addChild(*normalShader); TODO Delete after full refactor.
        appState->scene.addStep(ActivateShader(normalShader));

        auto &wireframeShader = appState->scene.addSceneNode(Shader("wireframe",
                                        IOUtils::getResource("shaders/debug/normalDebug.vert"),
                                        IOUtils::getResource("shaders/debug/wireframe.geom"),
                                        IOUtils::getResource("shaders/debug/normalDebug.frag")));
        wireframeShader.setActive(false);
        appState->scene.addStep(ActivateShader(wireframeShader));

        auto &shader = appState->scene.addSceneNode(Shader("blinnPhong",
                              IOUtils::getResource("shaders/phong/basicBlinnPhong.vert"),
                              IOUtils::getResource("shaders/phong/basicBlinnPhong.frag")));
        appState->scene.addStep(ActivateShader(shader));

        auto &camera = appState->scene.addSceneNode(Camera("camera", SCR_WIDTH - gui.getGuiWidth(),
                                SCR_HEIGHT, CameraMode::ANCHOR,glm::vec3(0.0f, 0.0f, 3.0f)));
        appState->scene.addStep(AddCamera(camera));
        appState->currentCamera = camera;

//        auto gridShader = std::make_unique<Shader>("grid", IOUtils::getResource("shaders/debug/grid.vert"),
//                                                   IOUtils::getResource("shaders/debug/grid.frag"));
//        auto grid = ModelGenerator::generatePlaneMesh(glm::vec3(0,1,0));
//        gridShader->addChild(*grid);

        auto &pointLight = appState->scene.addSceneNode(Light("pointLight", glm::vec3(1.0, 1.0f, 1.0f)));
        appState->scene.addStep(AddLight(pointLight));

        auto axis = ModelGenerator::generateAxis();
        appState->scene.merge(std::move(axis));

        AssetImporter assetImporter;
        auto model = assetImporter.importModel(IOUtils::getResource("models/spitfire_mini/model/model.gltf"));


        while (openGlInstance.isRunning()) {
            OpenGLInstance::pollEvents();
            gui.newFrame();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(gui.getGuiWidth(), 0, SCR_WIDTH - gui.getGuiWidth(), SCR_HEIGHT);
            ExecuteStepVisitor executeStepVisitor;
            RenderingStep::visitTree(appState->scene.modelStepRoot->root, executeStepVisitor);

            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            gui.render();

            openGlInstance.swapBuffers();
        }
    }

    glfwTerminate();
    return 0;
}
