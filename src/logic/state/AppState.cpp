//
// Created by faliszewskii on 17.02.24.
//

#include "../../presentation/modules/renderers/GridModule.h"
#include "AppState.h"
#include "../../presentation/modules/gui/DebugOverlayModule.h"
#include "../../presentation/modules/gui/MenuBarModule.h"
#include "../../presentation/modules/gui/WorkspaceModule.h"

AppState::AppState(Rect<int> viewport, int guiPanelLeftWidth) :
        guiFocus(true),
        vSync(false),
        transformTree("root"),
        menuBarModule(std::make_unique<MenuBarModule>()),
        logPanelModule(std::make_unique<LogPanelModule>()),
        debugOverlayModule(std::make_unique<DebugOverlayModule>(guiPanelLeftWidth)),
        gizmoModule(std::make_unique<GizmoModule>(guiPanelLeftWidth)),
        workspaceModule(std::make_unique<WorkspaceModule>(guiPanelLeftWidth)),
        normalRendererModule(std::make_unique<NormalRenderModule>(guiPanelLeftWidth)),
        phongRendererModule(std::make_unique<PhongRenderModule>(guiPanelLeftWidth)),
        randomRendererModule(std::make_unique<RandomRenderModule>(guiPanelLeftWidth)),
        gridModule(new GridModule(guiPanelLeftWidth))
        {}

void AppState::runModules() {
    if(menuBarModule->active) menuBarModule->run(*this);
    if(debugOverlayModule->active) debugOverlayModule->run(*this);
    if(logPanelModule->active) logPanelModule->run(*this);
    if(workspaceModule->active) workspaceModule->run(*this);
    if(gizmoModule->active) gizmoModule->run(*this);
    if(normalRendererModule->active) normalRendererModule->run(*this);
    if(phongRendererModule->active) phongRendererModule->run(*this);
    if(randomRendererModule->active) randomRendererModule->run(*this);
    if(gridModule->active) gridModule->run(*this);
}
