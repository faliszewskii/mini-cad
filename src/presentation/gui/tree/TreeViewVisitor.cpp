//
// Created by faliszewskii on 07.01.24.
//

#include "TreeViewVisitor.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/frameBuffer/FrameBuffer.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/camera/Camera.h"

TreeViewVisitor::TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>> &selectedNode, int &selectedProperty)
        : nodeOpenStack(std::stack<bool>()), selectedNode(selectedNode), selectedProperty(selectedProperty) {
    flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;
}

bool TreeViewVisitor::renderTreeNode(ImGuiTreeNodeFlags localFlags, SceneNode &sceneNode, const char *fmt, bool leaf) {
    if (selectedNode && selectedNode.value().get() == sceneNode) localFlags |= ImGuiTreeNodeFlags_Selected;
    bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(sceneNode.getUuid()).c_str(), localFlags, fmt,
                                      sceneNode.getName().c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selectedNode = sceneNode;
        selectedProperty = 0;
    }
    if (!leaf) {
        nodeOpenStack.push(nodeOpen);
    }
    return nodeOpen;
}

int TreeViewVisitor::visitFrameBuffer(FrameBuffer &frameBuffer) {
    if (renderTreeNode(flags, frameBuffer, "[F] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitTransformation(Transformation &transformation) {
    if (renderTreeNode(flags, transformation, "[T] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitShader(Shader &shader) {
    if (renderTreeNode(flags, shader, "[S] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitMaterial(Material &material) {
    if (renderTreeNode(flags, material, "[Ma] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitLight(Light &light) {
    if (renderTreeNode(flags, light, "[L] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitCamera(Camera &camera) {
    if (renderTreeNode(flags, camera, "[C] %s")) return 0;
    return 1;
}

int TreeViewVisitor::visitMesh(Mesh &mesh) {
    ImGuiTreeNodeFlags localFlags = flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(localFlags, mesh, "[Me] %s", true);
    return 0;
}

int TreeViewVisitor::treePop() {
    if (nodeOpenStack.top())
        ImGui::TreePop();
    nodeOpenStack.pop();
    return 0;
}
