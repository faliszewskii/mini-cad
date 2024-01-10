//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TREEVIEWVISITOR_H
#define OPENGL_SANDBOX_TREEVIEWVISITOR_H


#include <stack>
#include "imgui.h"
#include "../../scene/visitor/SceneNodeVisitor.h"

class TreeViewVisitor : public SceneNodeVisitor {
    std::stack<bool> nodeOpenStack;
    ImGuiTreeNodeFlags flags;
    // TODO Add icons to gui to identify SceneNode type
public:
    TreeViewVisitor();

    int visitTransformation(Transformation& transformation) override;
    int visitMesh(Mesh& mesh) override;
    int visitLight(Light& light) override;
    int visitPointLight(PointLight& pointLight) override;
    int visitCamera(Camera& camera) override;

    int leaveTransformation(Transformation& transformation) override;
};


#endif //OPENGL_SANDBOX_TREEVIEWVISITOR_H
