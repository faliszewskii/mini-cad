//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TREEVIEWVISITOR_H
#define OPENGL_SANDBOX_TREEVIEWVISITOR_H


#include <stack>
#include <optional>
#include "imgui.h"
#include "../../scene/visitor/SceneNodeVisitor.h"
#include "../../scene/tree/SceneNode.h"

class TreeViewVisitor : public SceneNodeVisitor {
    std::stack<bool> nodeOpenStack;
    ImGuiTreeNodeFlags flags;
    std::optional<std::reference_wrapper<SceneNode>> &selectedNode;
    // TODO Add icons to gui to identify SceneNode type
public:
    explicit TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>> &selectedNode);

    int visitFrameBuffer(FrameBuffer &frameBuffer) override;
    int visitTransformation(Transformation &transformation) override;
    int visitMesh(Mesh &mesh) override;
    int visitLight(Light &light) override;
    int visitCamera(Camera &camera) override;
    int visitShader(Shader &shader) override;
    int visitMaterial(Material &material) override;

    int leaveFrameBuffer(FrameBuffer &frameBuffer) override { return treePop(); };
    int leaveTransformation(Transformation &transformation) override { return treePop(); };
    int leaveLight(Light &light) override { return treePop(); };
    int leaveCamera(Camera &camera) override { return treePop(); };
    int leaveShader(Shader &shader) override { return treePop(); }
    int leaveMaterial(Material &material) override { return treePop(); }

    bool renderTreeNode(ImGuiTreeNodeFlags localFlags, SceneNode &sceneNode, const char *fmt, bool leaf = false);

    int treePop();
};


#endif //OPENGL_SANDBOX_TREEVIEWVISITOR_H
