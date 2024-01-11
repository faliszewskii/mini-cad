//
// Created by faliszewskii on 08.01.24.
//

#include <iostream>
#include "PropertyViewVisitor.h"
#include "imgui.h"
#include "../../properties/transformation/TransformationProperty.h"
#include "glm/gtc/type_ptr.hpp"

void PropertyViewVisitor::visitTransformationParameter(TransformationProperty &transformation) {
    ImGui::Text("Transformation Property");
    ImGui::DragFloat("x", static_cast<float*>(glm::value_ptr(transformation.getPositionRef()))+0, 0.01f);
    ImGui::DragFloat("y", static_cast<float*>(glm::value_ptr(transformation.getPositionRef()))+1, 0.01f);
    ImGui::DragFloat("z", static_cast<float*>(glm::value_ptr(transformation.getPositionRef()))+2, 0.01f);
    ImGui::DragFloat4("orientation", static_cast<float*>(glm::value_ptr(transformation.getOrientationRef())), 0.01f);
    ImGui::DragFloat3("scale", static_cast<float*>(glm::value_ptr(transformation.getScaleRef())), 0.01f);
}
