//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_SCENETREENODE_H
#define OPENGL_SANDBOX_SCENETREENODE_H


#include <vector>
#include "SceneNode.h"

class SceneTreeNode {
    std::unique_ptr<SceneNode> value;
    std::vector<std::unique_ptr<SceneTreeNode>> children;

public :
    explicit SceneTreeNode(std::unique_ptr<SceneNode> value);

    std::vector<std::unique_ptr<SceneTreeNode>>& getChildren() { return children; };
    std::unique_ptr<SceneNode>& getValue() { return value; };
    void visitTree(SceneNodeVisitor &visitor) const;

    void addChild(std::unique_ptr<SceneNode> childValue);
    void addChild(std::unique_ptr<SceneTreeNode> childNode);
};


#endif //OPENGL_SANDBOX_SCENETREENODE_H
