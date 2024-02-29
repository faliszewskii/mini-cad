//
// Created by faliszewskii on 17.02.24.
//

#include "../../presentation/modules/renderers/VerticalStripedLineModule.h"
#include "../../presentation/modules/renderers/RayCastingModule.h"
#include "../../presentation/modules/renderers/GridModule.h"
#include "AppState.h"
#include "../../presentation/modules/gui/DebugOverlayModule.h"
#include "../../presentation/modules/gui/MenuBarModule.h"
#include "../../presentation/modules/gui/WorkspaceModule.h"

AppState::AppState(Rect<int> viewport, int guiPanelLeftWidth) :
        guiFocus(true),
        vSync(false),
        transformTree("root"),
        menuBarModule(std::make_unique<MenuBarModule>(true)),
        logPanelModule(std::make_unique<LogPanelModule>(true)),
        debugOverlayModule(std::make_unique<DebugOverlayModule>(guiPanelLeftWidth, true)),
        gizmoModule(std::make_unique<GizmoModule>(guiPanelLeftWidth, true)),
        workspaceModule(std::make_unique<WorkspaceModule>(guiPanelLeftWidth, true)),
        normalRendererModule(std::make_unique<NormalRenderModule>(guiPanelLeftWidth, false)),
        phongRendererModule(std::make_unique<PhongRenderModule>(guiPanelLeftWidth, false)),
        randomRendererModule(std::make_unique<RandomRenderModule>(guiPanelLeftWidth, false)),
        wireframeRendererModule(std::make_unique<WireframeRenderModule>(guiPanelLeftWidth, true)),
        gridModule(new GridModule(guiPanelLeftWidth, true)),
        rayCastingModule(std::make_unique<RayCastingModule>(guiPanelLeftWidth, false)),
        verticalStripedLineModule(std::make_unique<VerticalStripedLineModule>(guiPanelLeftWidth, true))
        {}

void AppState::runModules() {
    if(normalRendererModule->active) normalRendererModule->run(*this);
    if(phongRendererModule->active) phongRendererModule->run(*this);
    if(randomRendererModule->active) randomRendererModule->run(*this);
    if(wireframeRendererModule->active) wireframeRendererModule->run(*this);
    if(rayCastingModule->active) rayCastingModule->run(*this);
    if(verticalStripedLineModule->active) verticalStripedLineModule->run(*this);

    // Grid has to be at the end to maintain transparency.
    if(gridModule->active) gridModule->run(*this);


    // UI
    if(workspaceModule->active) workspaceModule->run(*this);
    if(menuBarModule->active) menuBarModule->run(*this);
    if(debugOverlayModule->active) debugOverlayModule->run(*this);
    if(logPanelModule->active) logPanelModule->run(*this);
    if(gizmoModule->active) gizmoModule->run(*this);
}
