
#include "../../lib/glad/glad_glfw.h"
#define STB_IMAGE_IMPLEMENTATION
#include "logic/io/stb_image.h"

#include "presentation/rendering/shader/shader.h"
#include "presentation/camera/camera.h"
#include "logic/opengl/OpenGLInstance.h"
#include "logic/io/IOUtils.h"
#include "logic/importer/AssetImporter.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "logic/generator/ModelGenerator.h"
#include "presentation/gui/GUI.h"
#include "logic/ApplicationState.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool cameraMode = false;

// camera
Camera cameraAnchor(CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f));
Camera cameraFree(CameraMode::FREE, glm::vec3(0.0f, 0.0f, 3.0f));
Camera *currentCamera = &cameraAnchor;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    OpenGLInstance openGlInstance{};
    openGlInstance.init(SCR_WIDTH, SCR_HEIGHT, mouse_callback, mouse_button_callback, scroll_callback);

    // TODO Do research on paths in C++. Try to be as OS agnostic as possible. Cerberus model had the 'windows slash' problem
    Shader ourShader(IOUtils::getResource("shaders/t.vert").c_str(), IOUtils::getResource("shaders/t.frag").c_str());

    AssetImporter assetImporter;

    ApplicationState appState;

    appState.rootModelNode.getChildren().push_back(ModelGenerator::generateAxis());

    auto result = assetImporter.importModel(IOUtils::getResource("models/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX"));
    if(result) appState.rootModelNode.getChildren().push_back(result.value());

    result = assetImporter.importModel(IOUtils::getResource("models/spitfire_mini/model/model.gltf"));
    if(result) appState.rootModelNode.getChildren().push_back(result.value());

    GUI gui(openGlInstance.getWindow(), appState);

    // render loop
    // -----------
    while (openGlInstance.isRunning())
    {
        openGlInstance.pollEvents();
        gui.newFrame();


        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(openGlInstance.getWindow());


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(ZOOM), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = currentCamera->getViewMatrix();
        ourShader.setMat4("view", view);

//        axis->draw(ourShader,  glm::translate(glm::mat4(1.0f), currentCamera->anchor));
        appState.rootModelNode.render(ourShader, glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));

        gui.render();

        openGlInstance.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (ImGui::GetIO().WantCaptureKeyboard) return;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        currentCamera = &cameraFree;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        currentCamera = &cameraAnchor;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        currentCamera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        currentCamera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        currentCamera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        currentCamera->processKeyboard(RIGHT, deltaTime);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        cameraMode = true;
    else
        cameraMode = false;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;


    if (cameraMode) {
        currentCamera->processMouseMovement(xoffset, yoffset);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    currentCamera->processMouseScroll(static_cast<float>(yoffset));
}