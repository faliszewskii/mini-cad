//
// Created by faliszewskii on 01.02.24.
//

#ifndef OPENGL_SANDBOX_STEPTREE_H
#define OPENGL_SANDBOX_STEPTREE_H

#include <vector>
#include "../steps/RenderingStep.h"

class StepTreeNode {
    std::unique_ptr<RenderingStep> step;
    std::vector<std::unique_ptr<StepTreeNode>> children;

public:
    explicit StepTreeNode(std::unique_ptr<RenderingStep> step) : step(std::move(step)) {};

    std::vector<std::unique_ptr<StepTreeNode>> &getChildren() { return children; };

    template<typename T>
    StepTreeNode& addChild(T &&childStep) {
        children.push_back(std::make_unique<StepTreeNode>(std::make_unique<T>(childStep)));
        return *children[children.size()-1];
    };
    StepTreeNode& addChild(std::unique_ptr<StepTreeNode> childStep) {
        children.push_back(std::move(childStep));
        return *children[children.size()-1];
    };

    static void visitTree(StepTreeNode &treeNode, StepVisitor &visitor) {
        std::stack<std::pair<std::reference_wrapper<StepTreeNode>, int>> stack{};
        stack.emplace(treeNode, 0);
        while (!stack.empty()) {
            StepTreeNode &node = stack.top().first;
            int state = stack.top().second;
            stack.pop();
            if (state == 0) {
                stack.emplace(node, 1);
                if (!node.step->acceptVisit(visitor))
                    for (auto &child: std::ranges::views::reverse(node.children))
                        stack.emplace(*child, 0);
            }
            if (state == 1) node.step->acceptLeave(visitor);
        }
    };
};

class StepTree {
public:
    RootStep root;

    template<typename T>
    void addChild(T &&childStep) { root.addChild(std::forward<T>(childStep)); };
};

#endif //OPENGL_SANDBOX_STEPTREE_H
