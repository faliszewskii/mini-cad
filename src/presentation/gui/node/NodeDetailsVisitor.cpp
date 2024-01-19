//
// Created by faliszewskii on 11.01.24.
//

#include <glm/gtc/type_ptr.hpp>
#include "NodeDetailsVisitor.h"
#include "../../scene/nodes/shader/Shader.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/material/Material.h"
#include "imgui.h"

int NodeDetailsVisitor::visitTransformation(Transformation &transformation) {
    ImGui::Text("Position:");
// TODO grey-out on bind.
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(transformation.getTranslationRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(transformation.getTranslationRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(transformation.getTranslationRef())) + 2, 0.01f);
    ImGui::Text("Orientation:");
    ImGui::DragFloat("x##orientation", static_cast<float *>(glm::value_ptr(transformation.getOrientationRef())) + 0,
                     0.01f);
    ImGui::DragFloat("y##orientation", static_cast<float *>(glm::value_ptr(transformation.getOrientationRef())) + 1,
                     0.01f);
    ImGui::DragFloat("z##orientation", static_cast<float *>(glm::value_ptr(transformation.getOrientationRef())) + 2,
                     0.01f);
    ImGui::DragFloat("w", static_cast<float *>(glm::value_ptr(transformation.getOrientationRef())) + 3, 0.01f);
    ImGui::Text("Scale:");
    ImGui::DragFloat("x##scale", static_cast<float *>(glm::value_ptr(transformation.getScaleRef())) + 0, 0.001f);
    ImGui::DragFloat("y##scale", static_cast<float *>(glm::value_ptr(transformation.getScaleRef())) + 1, 0.001f);
    ImGui::DragFloat("z##scale", static_cast<float *>(glm::value_ptr(transformation.getScaleRef())) + 2, 0.001f);
}

int NodeDetailsVisitor::visitShader(Shader &shader) {
    if (ImGui::SmallButton("Hot reload"))
        shader.hotReload();
    return 0;
}

int NodeDetailsVisitor::visitLight(Light &light) {
    ImGui::DragFloat("strength", &light.getStrengthRef(), 0.01f);

    ImGui::Text("Position:");
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(light.getPositionRef())) + 2, 0.01f);

    ImGui::Text("Color:");
    ImGui::DragFloat("r##light", static_cast<float *>(glm::value_ptr(light.getColorRef())) + 0, 0.01f, 0, 1);
    ImGui::DragFloat("g##light", static_cast<float *>(glm::value_ptr(light.getColorRef())) + 1, 0.01f, 0, 1);
    ImGui::DragFloat("b##light", static_cast<float *>(glm::value_ptr(light.getColorRef())) + 2, 0.01f, 0, 1);

    return 0;
}

int NodeDetailsVisitor::visitMaterial(Material &material) {
    if (!material.getHint().empty())
        ImGui::Text("AssImp hint: %s", material.getHint().c_str());
    ImGui::DragFloat("shininess", &material.getShininessRef(), 0.01f, 0); // TODO To Phong property.
    return 0;
}

