//
// Created by faliszewskii on 17.02.24.
//

#include "../../presentation/modules/renderers/VerticalStripedLineModule.h"
#include "../../presentation/modules/renderers/GridModule.h"
#include "AppState.h"
#include "../../presentation/modules/gui/DebugOverlayModule.h"
#include "../../presentation/modules/gui/MenuBarModule.h"
#include "../../presentation/modules/gui/WorkspaceModule.h"

AppState::AppState(Rect<int> viewport, int guiPanelLeftWidth) :
        camera(viewport.width, viewport.height, CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f)),
        guiFocus(true),
        vSync(true),
        keyboardCtrlMode(false),
        guiPanelLeftWidth(guiPanelLeftWidth),
        menuBarModule(std::make_unique<MenuBarModule>()),
        logPanelModule(std::make_unique<LogPanelModule>()),
        debugOverlayModule(std::make_unique<DebugOverlayModule>(guiPanelLeftWidth)),
        gizmoModule(std::make_unique<GizmoModule>(guiPanelLeftWidth)),
        workspaceModule(std::make_unique<WorkspaceModule>(guiPanelLeftWidth)),
        wireframeRendererModule(std::make_unique<WireframeRenderModule>(guiPanelLeftWidth)),
        gridModule(new GridModule(guiPanelLeftWidth)),
        verticalStripedLineModule(std::make_unique<VerticalStripedLineModule>(guiPanelLeftWidth)) {
        glfwSwapInterval(vSync);
    }

void AppState::runModules() {
    wireframeRendererModule->run(*this);
    verticalStripedLineModule->run(*this);

    // Grid has to be at the end to maintain transparency.
    gridModule->run(*this);

    // UI
    workspaceModule->run(*this);
    menuBarModule->run(*this);
    debugOverlayModule->run(*this);
    logPanelModule->run(*this);
    gizmoModule->run(*this);
}
