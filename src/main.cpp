
#include <memory>
#define _USE_MATH_DEFINES
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "logic/io/stb_image.h"
#include "../../lib/glad/glad_glfw.h"
#include "logic/opengl/OpenGLInstance.h"
#include "presentation/gui/GUI.h"
#include "logic/state/AppState.h"
#include "logic/input/InputHandler.h"
#include "presentation/modules/gui/WorkspaceModule.h"
#include "presentation/modules/gui/MenuBarModule.h"
#include "presentation/modules/gui/LogPanelModule.h"
#include "presentation/modules/gui/DebugOverlayModule.h"
#include "presentation/modules/gui/GizmoModule.h"
#include "presentation/modules/renderers/GridModule.h"
#include "presentation/modules/renderers/WireframeRenderModule.h"
#include "presentation/modules/renderers/VerticalStripedLineModule.h"
#include "presentation/modules/renderers/CursorModule.h"
#include "presentation/modules/renderers/CenterOfMassModule.h"

// settings
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;
const int GUI_PANEL_LEFT_WIDTH = 300;

std::unique_ptr<AppState> initializeAppState() {
    const auto &viewport = Rect<int>(GUI_PANEL_LEFT_WIDTH, 0, SCR_WIDTH - GUI_PANEL_LEFT_WIDTH, SCR_HEIGHT);

    auto appState = std::make_unique<AppState>(viewport, GUI_PANEL_LEFT_WIDTH);

    return appState;
}


int main() {
    OpenGLInstance openGlInstance{};
    openGlInstance.init(SCR_WIDTH, SCR_HEIGHT);

    auto appState = initializeAppState();

    InputHandler inputHandler(appState);
    inputHandler.setupCallbacks(openGlInstance.getWindow());

    {
        GUI gui(*appState, openGlInstance.getWindow());
        while (openGlInstance.isRunning()) {
            OpenGLInstance::pollEvents();
            gui.newFrame();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            appState->runModules();

            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            ImGui::ShowDemoWindow();
            gui.render();

            openGlInstance.swapBuffers();
        }
    }

    glfwTerminate();
    return 0;
}
