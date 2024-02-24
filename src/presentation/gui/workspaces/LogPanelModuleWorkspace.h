//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_LOGPANELMODULEWORKSPACE_H
#define OPENGL_SANDBOX_LOGPANELMODULEWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "ModulesWorkspaceHelpers.h"
#include "../../modules/gui/LogPanelModule.h"

namespace LogPanelModuleWorkspace {
    inline void render(AppState &appState) {
        ModulesWorkspaceHelpers::renderModuleToggle(appState.logPanelModule->active);
    }
};

#endif //OPENGL_SANDBOX_LOGPANELMODULEWORKSPACE_H
