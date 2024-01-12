//
// Created by faliszewskii on 11.01.24.
//

#include "NodeDetailsVisitor.h"
#include "../../scene/nodes/shader/Shader.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/material/Material.h"
#include "imgui.h"

int NodeDetailsVisitor::visitShader(Shader &shader) {
    if(ImGui::SmallButton("Hot reload"))
        shader.hotReload();
    return 0;
}

int NodeDetailsVisitor::visitLight(Light &light) {
    ImGui::DragFloat("strength", &light.strength, 0.01f);
    return 0;
}

int NodeDetailsVisitor::visitMaterial(Material &material) {
    if(!material.getHint().empty())
        ImGui::Text("AssImp hint: %s", material.getHint().c_str());
    return 0;
}

