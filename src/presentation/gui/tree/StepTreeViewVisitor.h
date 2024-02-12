//
// Created by faliszewskii on 02.02.24.
//

#ifndef OPENGL_SANDBOX_STEPTREEVIEWVISITOR_H
#define OPENGL_SANDBOX_STEPTREEVIEWVISITOR_H


#include "../../scene/visitor/StepVisitor.h"
#include "../../scene/steps/RenderingStep.h"
#include "imgui.h"

class StepTreeViewVisitor : public StepVisitor {
    std::stack<bool> nodeOpenStack;
    ImGuiTreeNodeFlags flags;
    std::optional<std::reference_wrapper<RenderingStep>> &selectedStep;
public:
    explicit StepTreeViewVisitor(std::optional<std::reference_wrapper<RenderingStep>> &selectedNode);
    bool renderTreeNode(ImGuiTreeNodeFlags localFlags, RenderingStep &step, const std::string& nodeName, bool leaf = false);
    bool renderTreeLeaf(ImGuiTreeNodeFlags flags, RenderingStep &step, const std::string &nodeName);
    int treePop();

    int visitRootStep(RootStep &step) override;
    int leaveRootStep(RootStep &step) override { return treePop(); };

    int visitAddFrameBuffer(AddFrameBuffer &step) override;

    int visitPushTransformation(PushTransformation &step) override;
    int leavePushTransformation(PushTransformation &step) override { return treePop(); };

    int visitAddCamera(AddCamera &step) override;

    int visitAddLight(AddLight &step) override;

    int visitAddMaterial(AddMaterial &step) override;

    int visitActivateShader(ActivateShader &step) override;

    int visitDrawMesh(DrawMesh &step) override;
};


#endif //OPENGL_SANDBOX_STEPTREEVIEWVISITOR_H
