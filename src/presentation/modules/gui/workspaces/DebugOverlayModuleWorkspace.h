//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_DEBUGOVERLAYMODULEWORKSPACE_H
#define OPENGL_SANDBOX_DEBUGOVERLAYMODULEWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "../DebugOverlayModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace DebugOverlayModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.debugOverlayModule->active);
    }
};

#endif //OPENGL_SANDBOX_DEBUGOVERLAYMODULEWORKSPACE_H
