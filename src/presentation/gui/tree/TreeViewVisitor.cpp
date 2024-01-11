//
// Created by faliszewskii on 07.01.24.
//

#include "TreeViewVisitor.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/light/pointLight/PointLight.h"
#include "../../scene/nodes/camera/Camera.h"

TreeViewVisitor::TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>>& selectedNode) :
nodeOpenStack(std::stack<bool>()), selectedNode(selectedNode) {
    flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;
}

int TreeViewVisitor::visitTransformation(Transformation &transformation) {
    ImGuiTreeNodeFlags localFlags = flags;
    if (selectedNode && selectedNode.value().get() == transformation) localFlags |= ImGuiTreeNodeFlags_Selected;
    bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(transformation.getUuid()).c_str(), localFlags, "[T]%s", transformation.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = transformation;
    nodeOpenStack.push(nodeOpen);
    flags &= ~ImGuiTreeNodeFlags_DefaultOpen;
    if(nodeOpen) return 0;
    return 1;
}

int TreeViewVisitor::visitMesh(Mesh &mesh) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (selectedNode && selectedNode.value().get() == mesh) localFlags |= ImGuiTreeNodeFlags_Selected;
    ImGui::TreeNodeEx(uuids::to_string(mesh.getUuid()).c_str(), localFlags, "[M]%s", mesh.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = mesh;
    return 0;
}

int TreeViewVisitor::visitLight(Light &light) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (selectedNode && selectedNode.value().get() == light) localFlags |= ImGuiTreeNodeFlags_Selected;
    ImGui::TreeNodeEx(uuids::to_string(light.getUuid()).c_str(), localFlags, "[L]%s", light.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = light;
    return 0;
}

int TreeViewVisitor::visitPointLight(PointLight &pointLight) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (selectedNode && selectedNode.value().get() == pointLight) localFlags |= ImGuiTreeNodeFlags_Selected;
    ImGui::TreeNodeEx(uuids::to_string(pointLight.getUuid()).c_str(), localFlags, "[PL]%s", pointLight.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = pointLight;
    return 0;
}

int TreeViewVisitor::visitCamera(Camera &camera) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (selectedNode && selectedNode.value().get() == camera) localFlags |= ImGuiTreeNodeFlags_Selected;
    ImGui::TreeNodeEx(uuids::to_string(camera.getUuid()).c_str(), localFlags, "[C]%s", camera.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = camera;
    return 0;
}

int TreeViewVisitor::leaveTransformation(Transformation &transformation) {
    if(nodeOpenStack.top())
        ImGui::TreePop();
    nodeOpenStack.pop();
    return 0;
}
