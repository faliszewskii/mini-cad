//
// Created by faliszewskii on 07.01.24.
//

#include <stack>
#include <ranges>
#include "SceneTreeNode.h"
#include "../nodes/transformation/Transformation.h"


SceneTreeNode::SceneTreeNode(std::unique_ptr<SceneNode> value) : value(std::move(value)),
                                                                 children(std::vector<std::unique_ptr<SceneTreeNode>>()) {}

void SceneTreeNode::visitTree(SceneNodeVisitor &visitor) const {
    std::stack<std::pair<std::reference_wrapper<const SceneTreeNode>, int>> stack{};
    stack.emplace(std::cref(*this), 0);
    while(!stack.empty()) {
        const SceneTreeNode& node = stack.top().first;
        int state = stack.top().second;
        stack.pop();
        if (state == 0) {
            stack.emplace(std::cref(node), 1);
            if(!node.value->acceptVisit(visitor))
                for(auto &child : std::ranges::views::reverse(node.children))
                    stack.emplace(std::cref(*child), 0);
        }
        if (state == 1) node.value->acceptLeave(visitor);

    }
}

void SceneTreeNode::addChild(std::unique_ptr<SceneNode> childValue) {
    children.push_back(std::make_unique<SceneTreeNode>(SceneTreeNode(std::move(childValue))));
}

void SceneTreeNode::addChild(std::unique_ptr<SceneTreeNode> childNode) {
    children.push_back(std::move(childNode));
}
