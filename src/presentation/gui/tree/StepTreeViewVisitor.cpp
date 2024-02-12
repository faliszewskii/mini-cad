//
// Created by faliszewskii on 02.02.24.
//

#include "StepTreeViewVisitor.h"

StepTreeViewVisitor::StepTreeViewVisitor(std::optional<std::reference_wrapper<RenderingStep>> &selectedStep)
        : nodeOpenStack(std::stack<bool>()), selectedStep(selectedStep) {
    flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;
}

bool StepTreeViewVisitor::renderTreeNode(ImGuiTreeNodeFlags localFlags, RenderingStep &step, const std::string& nodeName, bool leaf) {
    if (selectedStep && selectedStep.value().get() == step) localFlags |= ImGuiTreeNodeFlags_Selected;
    bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(step.getUuid()).c_str(), localFlags, "[%s] %s",
                                      step.getTypeName().c_str(), nodeName.c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        selectedStep = step;
    if (!leaf)
        nodeOpenStack.push(nodeOpen);
    return nodeOpen;
}

bool StepTreeViewVisitor::renderTreeLeaf(ImGuiTreeNodeFlags localFlags, RenderingStep &step, const std::string& nodeName) {
    ImGuiTreeNodeFlags leafFlags = localFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    renderTreeNode(leafFlags, step, nodeName, true);
    return 0;
}

int StepTreeViewVisitor::treePop() {
    if (nodeOpenStack.top())
        ImGui::TreePop();
    nodeOpenStack.pop();
    return 0;
}


int StepTreeViewVisitor::visitRootStep(RootStep &step) {
    if (renderTreeNode(flags, step, step.getTypeName())) return 0;
    return 1;
}

int StepTreeViewVisitor::visitAddFrameBuffer(AddFrameBuffer &step) {
    return renderTreeLeaf(flags, step, step.frameBuffer.getName());
}

int StepTreeViewVisitor::visitPushTransformation(PushTransformation &step) {
    if (renderTreeNode(flags, step, step.transformation.getName())) return 0;
    return 1;
}

int StepTreeViewVisitor::visitAddCamera(AddCamera &step) {
    return renderTreeLeaf(flags, step, step.camera.getName());
}

int StepTreeViewVisitor::visitAddLight(AddLight &step) {
    return renderTreeLeaf(flags, step, step.light.getName());
}

int StepTreeViewVisitor::visitAddMaterial(AddMaterial &step) {
    return renderTreeLeaf(flags, step, step.material.getName());
}

int StepTreeViewVisitor::visitActivateShader(ActivateShader &step) {
    return renderTreeLeaf(flags, step, step.shader.getName());

}

int StepTreeViewVisitor::visitDrawMesh(DrawMesh &step) {
    return renderTreeLeaf(flags, step, step.mesh.getName());
}
