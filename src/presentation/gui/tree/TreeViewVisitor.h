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
    std::optional<std::reference_wrapper<SceneNode>>& selectedNode;
    int &selectedProperty;
    // TODO Add icons to gui to identify SceneNode type
public:
    explicit TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>>& selectedNode, int &selectedProperty);

    int visitTransformation(Transformation& transformation) override;
    int visitMesh(Mesh& mesh) override;
    int visitLight(Light& light) override;
    int visitCamera(Camera& camera) override;
    int visitShader(Shader &shader) override;

    int leaveTransformation(Transformation& transformation) override;

    bool renderTreeNode(ImGuiTreeNodeFlags localFlags, SceneNode &sceneNode, const char *fmt);
};


#endif //OPENGL_SANDBOX_TREEVIEWVISITOR_H
