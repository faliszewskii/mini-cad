//
// Created by faliszewskii on 07.01.24.
//

#include "TreeViewVisitor.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/camera/Camera.h"

TreeViewVisitor::TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>>& selectedNode, int &selectedProperty) :
nodeOpenStack(std::stack<bool>()), selectedNode(selectedNode), selectedProperty(selectedProperty) {
    flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;
}

bool TreeViewVisitor::renderTreeNode(ImGuiTreeNodeFlags localFlags, SceneNode& sceneNode, const char* fmt) {
    if (selectedNode && selectedNode.value().get() == sceneNode) localFlags |= ImGuiTreeNodeFlags_Selected;
    bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(sceneNode.getUuid()).c_str(), localFlags, fmt, sceneNode.getName().c_str());
    if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selectedNode = sceneNode;
        selectedProperty = 0;
    }
    return nodeOpen;
}

int TreeViewVisitor::visitTransformation(Transformation &transformation) {
    bool nodeOpen = renderTreeNode(flags, transformation, "[T] %s");
    nodeOpenStack.push(nodeOpen);
    flags &= ~ImGuiTreeNodeFlags_DefaultOpen;
    if(nodeOpen) return 0;
    return 1;
}

int TreeViewVisitor::leaveTransformation(Transformation &transformation) {
    if(nodeOpenStack.top())
        ImGui::TreePop();
    nodeOpenStack.pop();
    return 0;
}

int TreeViewVisitor::visitShader(Shader &shader) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(localFlags, shader, "[S] %s");
    return 0;
}

int TreeViewVisitor::visitMesh(Mesh &mesh) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(localFlags, mesh, "[M] %s");

    return 0;
}

int TreeViewVisitor::visitLight(Light &light) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(localFlags, light, "[L] %s");
    return 0;
}

int TreeViewVisitor::visitCamera(Camera &camera) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(localFlags, camera, "[C] %s");
    return 0;
}
