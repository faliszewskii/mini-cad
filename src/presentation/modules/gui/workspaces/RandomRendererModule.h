//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_RANDOMRENDERERMODULE_H
#define OPENGL_SANDBOX_RANDOMRENDERERMODULE_H

#include "../../../../logic/state/AppState.h"
#include "../../renderers/RandomRenderModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace RandomRenderModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.randomRendererModule->active);
    }
};

#endif //OPENGL_SANDBOX_RANDOMRENDERERMODULE_H
