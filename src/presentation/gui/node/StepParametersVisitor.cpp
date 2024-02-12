//
// Created by faliszewskii on 02.02.24.
//

#include <glm/gtc/type_ptr.hpp>
#include "StepParametersVisitor.h"
#include "../../scene/steps/RenderingStep.h"
#include "imgui.h"

int StepParametersVisitor::visitAddFrameBuffer(AddFrameBuffer &step) {
    drawHeader(step);
    return 0;
}

int StepParametersVisitor::visitPushTransformation(PushTransformation &step) {
    drawHeader(step);
    ImGui::Text("Position:");
// TODO grey-out on bind.
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(step.transformation.getTranslationRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(step.transformation.getTranslationRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(step.transformation.getTranslationRef())) + 2, 0.01f);
    ImGui::Text("Orientation:");
    ImGui::DragFloat("x##orientation", static_cast<float *>(glm::value_ptr(step.transformation.getOrientationRef())) + 0, 0.01f);
    ImGui::DragFloat("y##orientation", static_cast<float *>(glm::value_ptr(step.transformation.getOrientationRef())) + 1, 0.01f);
    ImGui::DragFloat("z##orientation", static_cast<float *>(glm::value_ptr(step.transformation.getOrientationRef())) + 2, 0.01f);
    ImGui::DragFloat("w##orientation", static_cast<float *>(glm::value_ptr(step.transformation.getOrientationRef())) + 3, 0.01f);
    ImGui::Text("Scale:");
    ImGui::DragFloat("x##scale", static_cast<float *>(glm::value_ptr(step.transformation.getScaleRef())) + 0, 0.001f);
    ImGui::DragFloat("y##scale", static_cast<float *>(glm::value_ptr(step.transformation.getScaleRef())) + 1, 0.001f);
    ImGui::DragFloat("z##scale", static_cast<float *>(glm::value_ptr(step.transformation.getScaleRef())) + 2, 0.001f);
    return 0;
}

int StepParametersVisitor::visitAddCamera(AddCamera &step) {
    drawHeader(step);
    return 0;
}

int StepParametersVisitor::visitAddLight(AddLight &step) {
    drawHeader(step);
    ImGui::DragFloat("strength", &step.light.getStrengthRef(), 0.01f);

    ImGui::Text("Position:");
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(step.light.getPositionRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(step.light.getPositionRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(step.light.getPositionRef())) + 2, 0.01f);

    ImGui::Text("Color:");
    ImGui::DragFloat("r##light", static_cast<float *>(glm::value_ptr(step.light.getColorRef())) + 0, 0.01f, 0, 1);
    ImGui::DragFloat("g##light", static_cast<float *>(glm::value_ptr(step.light.getColorRef())) + 1, 0.01f, 0, 1);
    ImGui::DragFloat("b##light", static_cast<float *>(glm::value_ptr(step.light.getColorRef())) + 2, 0.01f, 0, 1);

    ImGui::Text("Attenuation:");
    ImGui::DragFloat("1##Attenuation", &step.light.getConstantAttenuationRef(), 0.01f, 0, 1);
    ImGui::DragFloat("d##Attenuation", &step.light.getLinearAttenuationRef(), 0.01f, 0, 1);
    ImGui::DragFloat("dd##Attenuation", &step.light.getQuadraticAttenuationRef(), 0.01f, 0, 1);
    return 0;
}

int StepParametersVisitor::visitAddMaterial(AddMaterial &step) {
    drawHeader(step);
    if (!step.material.getHint().empty())
        ImGui::Text("AssImp hint: %s", step.material.getHint().c_str());
    ImGui::DragFloat("shininess", &step.material.getShininessRef(), 0.01f, 0); // TODO To Phong property.
    ImGui::ColorPicker4("albedo", static_cast<float *>(glm::value_ptr(step.material.getAlbedoRef())));
    return 0;
}

int StepParametersVisitor::visitActivateShader(ActivateShader &step) {
    drawHeader(step);
    if (ImGui::SmallButton("Hot reload"))
        step.shader.hotReload();
    if (!step.shader.active && ImGui::SmallButton("Turn On"))
        step.shader.setActive(true);
    else if (step.shader.active && ImGui::SmallButton("Turn Off"))
        step.shader.setActive(false);
    return 0;
}

int StepParametersVisitor::visitDrawMesh(DrawMesh &step) {
    drawHeader(step);
    return 0;
}

void StepParametersVisitor::drawHeader(RenderingStep &step) {
    ImGui::Text("%s parameters", step.getTypeName().c_str());
}
