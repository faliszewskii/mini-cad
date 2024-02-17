//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_NORMALRENDERERMODULEWORKSPACE_H
#define OPENGL_SANDBOX_NORMALRENDERERMODULEWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "../../renderers/NormalRenderModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace NormalRenderModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.normalRendererModule->active);
    }
};

#endif //OPENGL_SANDBOX_NORMALRENDERERMODULEWORKSPACE_H
