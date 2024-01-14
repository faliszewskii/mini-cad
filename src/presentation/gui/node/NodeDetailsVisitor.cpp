//
// Created by faliszewskii on 11.01.24.
//

#include <glm/gtc/type_ptr.hpp>
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
    ImGui::Text("Color:");
    ImGui::DragFloat("r##light", static_cast<float*>(glm::value_ptr(light.getColorRef()))+0, 0.01f, 0, 1);
    ImGui::DragFloat("g##light", static_cast<float*>(glm::value_ptr(light.getColorRef()))+1, 0.01f, 0, 1);
    ImGui::DragFloat("b##light", static_cast<float*>(glm::value_ptr(light.getColorRef()))+2, 0.01f, 0, 1);

    return 0;
}

int NodeDetailsVisitor::visitMaterial(Material &material) {
    if(!material.getHint().empty())
        ImGui::Text("AssImp hint: %s", material.getHint().c_str());
    ImGui::DragFloat("shininess", &material.shininess, 0.01f, 0); // TODO To Phong property.
    return 0;
}

