//
// Created by faliszewskii on 20.02.24.
//

#ifndef OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H
#define OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "../../modules/renderers/WireframeRenderModule.h"

namespace WireframeRenderModuleWorkspace {
    inline void render(AppState &appState) {
        if(ImGui::Button("Hot Reload")) {
            appState.wireframeRendererModule->hotReload();
        }
    }
};

#endif //OPENGL_SANDBOX_WIREFRAMERENDERMODULEWORKSPACE_H
