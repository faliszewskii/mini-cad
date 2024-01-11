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
    // TODO Add icons to gui to identify SceneNode type
public:
    TreeViewVisitor(std::optional<std::reference_wrapper<SceneNode>>& selectedNode);

    int visitTransformation(Transformation& transformation) override;
    int visitMesh(Mesh& mesh) override;
    int visitLight(Light& light) override;
    int visitPointLight(PointLight& pointLight) override;
    int visitCamera(Camera& camera) override;

    int leaveTransformation(Transformation& transformation) override;
};


#endif //OPENGL_SANDBOX_TREEVIEWVISITOR_H
