//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H

#include "../../../../logic/state/AppState.h"
#include "../../renderers/WireframeRenderModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace WireframeRenderModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.wireframeRendererModule->active);
    }
};

#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H
