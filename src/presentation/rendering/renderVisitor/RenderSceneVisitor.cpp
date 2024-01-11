//
// Created by faliszewskii on 07.01.24.
//

#include <glm/gtx/string_cast.hpp>
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/camera/Camera.h"
#include "RenderSceneVisitor.h"

RenderSceneVisitor::RenderSceneVisitor(){
    std::stack<glm::mat4> stack{};
    stack.emplace(1.0f);
    transformationStack = stack;
}

int RenderSceneVisitor::visitShader(Shader &shader) {
    activeShader = shader;
    shader.use();
}

int RenderSceneVisitor::visitTransformation(Transformation &transformation) {
    transformationStack.push(transformationStack.top() * transformation.getTransformation());
    return 0;
}

int RenderSceneVisitor::visitMesh(Mesh &mesh) {
    if(activeShader)
        mesh.render(activeShader->get(), transformationStack.top());
    return 0;
}

int RenderSceneVisitor::visitPointLight(PointLight &pointLight) {
    // Ignore this node
    // TODO Draw some representation of pointLight or light?
    return 0;
}


// settings // TODO decide what to do about screen width and perspective in camera.
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int RenderSceneVisitor::visitCamera(Camera &camera) {
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)camera.screenWidth / (float)camera.screenHeight, 0.1f, 100.0f); // TODO configurable
    if(activeShader) activeShader.value().get().setMat4("projection", projection);

    glm::mat4 view = camera.getViewMatrix();
    if(activeShader) activeShader.value().get().setMat4("view", view);

    // TODO Draw some representation of camera ?
    return 0;
}

int RenderSceneVisitor::leaveTransformation(Transformation &transformation) {
    transformationStack.pop();
    return 0;
}
