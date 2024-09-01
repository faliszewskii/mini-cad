//
// Created by faliszewskii on 17.02.24.
//

#include "../../presentation/modules/renderers/CenterOfMassModule.h"
#include "../../presentation/modules/renderers/CursorModule.h"
#include "../../presentation/modules/renderers/VerticalStripedLineModule.h"
#include "../../presentation/modules/renderers/GridModule.h"
#include "AppState.h"
#include "../../presentation/modules/gui/DebugOverlayModule.h"
#include "../../presentation/modules/gui/MenuBarModule.h"
#include "../../presentation/modules/gui/WorkspaceModule.h"
#include "../events/create/CreateEventsHandler.h"
#include "../events/selection/SelectionEventsHandler.h"
#include "../events/point/PointEventsHandler.h"
#include "../events/input/InputEventsHandler.h"

AppState::AppState(Rect<int> viewport, int guiPanelLeftWidth) :
        camera(viewport.width, viewport.height, CameraMode::ANCHOR, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-std::numbers::pi/4,0,0)),
        guiFocus(true),
        vSync(true),
        keyboardCtrlMode(false),
        bezierCreatorOpen(false),
        guiPanelLeftWidth(guiPanelLeftWidth),
        menuBarModule(std::make_unique<MenuBarModule>()),
        logPanelModule(std::make_unique<LogPanelModule>()),
        debugOverlayModule(std::make_unique<DebugOverlayModule>(guiPanelLeftWidth)),
        gizmoModule(std::make_unique<GizmoModule>(guiPanelLeftWidth)),
        workspaceModule(std::make_unique<WorkspaceModule>(guiPanelLeftWidth)),
        wireframeRendererModule(std::make_unique<WireframeRenderModule>(guiPanelLeftWidth)),
        gridModule(new GridModule(guiPanelLeftWidth)),
        verticalStripedLineModule(std::make_unique<VerticalStripedLineModule>(guiPanelLeftWidth)),
        cursorModule(std::make_unique<CursorModule>(guiPanelLeftWidth)),
        centerOfMassModule(std::make_unique<CenterOfMassModule>(guiPanelLeftWidth)),
        serializer(std::make_unique<Serializer>()){
        glfwSwapInterval(vSync);

        CreateEventsHandler::setup(*this);
        SelectionEventsHandler::setup(*this);
        PointEventsHandler::setup(*this);
        InputEventsHandler::setup(*this);
    }

void AppState::runModules() {
    wireframeRendererModule->run(*this);
    verticalStripedLineModule->run(*this);
    cursorModule->run(*this);
    centerOfMassModule->run(*this);
    // Grid has to be at the end to maintain transparency.
    gridModule->run(*this);

    // UI
    workspaceModule->run(*this);
    menuBarModule->run(*this);
    debugOverlayModule->run(*this);
    logPanelModule->run(*this);
    gizmoModule->run(*this);
}
