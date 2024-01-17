//
// Created by faliszewskii on 08.01.24.
//

#include <iostream>
#include "PropertyViewVisitor.h"
#include "imgui.h"
#include "../../properties/transformation/TransformationProperty.h"
#include "../../properties/light/PointLightProperty.h"
#include "glm/gtc/type_ptr.hpp"

void PropertyViewVisitor::visitTransformationProperty(TransformationProperty &transformation) {
    ImGui::SeparatorText("Transformation Property");
    ImGui::Text("Position:");
// TODO grey-out on bind.
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(transformation.getPositionRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(transformation.getPositionRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(transformation.getPositionRef())) + 2, 0.01f);
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

void PropertyViewVisitor::visitPointLightProperty(PointLightProperty &pointLight) {
    ImGui::SeparatorText("Point Light Property");
    ImGui::Text("Position:");
    ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(pointLight.getPositionRef())) + 0, 0.01f);
    ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(pointLight.getPositionRef())) + 1, 0.01f);
    ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(pointLight.getPositionRef())) + 2, 0.01f);
}
