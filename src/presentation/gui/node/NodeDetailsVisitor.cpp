//
// Created by faliszewskii on 11.01.24.
//

#include "NodeDetailsVisitor.h"
#include "../../scene/nodes/shader/Shader.h"
#include "imgui.h"

int NodeDetailsVisitor::visitShader(Shader &shader) {
    if(ImGui::SmallButton("Hot reload"))
        shader.hotReload();
    return 0;
}

