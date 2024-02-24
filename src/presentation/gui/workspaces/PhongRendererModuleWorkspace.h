//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_PHONGRENDERERMODULEWORKSPACE_H
#define OPENGL_SANDBOX_PHONGRENDERERMODULEWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "../../modules/renderers/PhongRenderModule.h"
#include "ModulesWorkspaceHelpers.h"

namespace PhongRenderModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.phongRendererModule->active);
    }
};

#endif //OPENGL_SANDBOX_PHONGRENDERERMODULEWORKSPACE_H
