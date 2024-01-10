//
// Created by faliszewskii on 07.01.24.
//

#include "TreeViewVisitor.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/light/pointLight/PointLight.h"
#include "../../scene/nodes/camera/Camera.h"

TreeViewVisitor::TreeViewVisitor() : nodeOpenStack(std::stack<bool>()){
    flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
}

int TreeViewVisitor::visitTransformation(Transformation &transformation) {
    bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(transformation.getUuid()).c_str(), flags, "[T]%s", transformation.getName().c_str());
    nodeOpenStack.push(nodeOpen);
    flags &= ~ImGuiTreeNodeFlags_DefaultOpen;
    if(nodeOpen) return 0;
    return 1;
}

int TreeViewVisitor::visitMesh(Mesh &mesh) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(uuids::to_string(mesh.getUuid()).c_str(), localFlags, "[M]%s", mesh.getName().c_str());
    return 0;
}

int TreeViewVisitor::visitLight(Light &light) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(uuids::to_string(light.getUuid()).c_str(), localFlags, "[L]%s", light.getName().c_str());
    return 0;
}

int TreeViewVisitor::visitPointLight(PointLight &pointLight) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(uuids::to_string(pointLight.getUuid()).c_str(), localFlags, "[PL]%s", pointLight.getName().c_str());
    return 0;
}

int TreeViewVisitor::visitCamera(Camera &camera) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(uuids::to_string(camera.getUuid()).c_str(), localFlags, "[C]%s", camera.getName().c_str());
    return 0;
}

int TreeViewVisitor::leaveTransformation(Transformation &transformation) {
    if(nodeOpenStack.top())
        ImGui::TreePop();
    nodeOpenStack.pop();
    return 0;
}
