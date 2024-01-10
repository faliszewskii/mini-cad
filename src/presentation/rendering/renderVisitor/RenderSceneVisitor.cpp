//
// Created by faliszewskii on 07.01.24.
//

#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "RenderSceneVisitor.h"

RenderSceneVisitor::RenderSceneVisitor(Shader& activeShader) : activeShader(activeShader) {
    std::stack<glm::mat4> stack{};
    stack.emplace(1.0f);
    transformationStack = stack;
}

int RenderSceneVisitor::visitTransformation(Transformation &transformation) {
    transformationStack.push(transformationStack.top() * transformation.getTransformation());
    return 0;
}

int RenderSceneVisitor::visitMesh(Mesh &mesh) {
    mesh.render(activeShader, transformationStack.top());
    return 0;
}

int RenderSceneVisitor::visitPointLight(PointLight &pointLight) {
    // Ignore this node
    // TODO Draw some representation of pointLight or light?
    return 0;
}

int RenderSceneVisitor::visitCamera(Camera &camera) {
    // Ignore this node
    // TODO Draw some representation of camera ?
    return 0;
}

int RenderSceneVisitor::leaveTransformation(Transformation &transformation) {
    transformationStack.pop();
    return 0;
}
