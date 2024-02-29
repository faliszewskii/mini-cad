//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_MODULESWORKSPACEHELPERS_H
#define OPENGL_SANDBOX_MODULESWORKSPACEHELPERS_H

#include "../../../logic/state/AppState.h"
#include "imgui.h"

namespace ModulesWorkspaceHelpers {
    inline bool renderModuleToggle(bool &active) {
        return ImGui::Checkbox("Toggle Module", &active);
    }
};

#endif //OPENGL_SANDBOX_MODULESWORKSPACEHELPERS_H
