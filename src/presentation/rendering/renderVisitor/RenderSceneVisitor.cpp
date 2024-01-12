//
// Created by faliszewskii on 07.01.24.
//

#include <glm/gtx/string_cast.hpp>
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/camera/Camera.h"
#include "RenderSceneVisitor.h"

RenderSceneVisitor::RenderSceneVisitor() : pointLightCounter(0) {
    std::stack<glm::mat4> stack{};
    stack.emplace(1.0f);
    transformationStack = stack;
}

int RenderSceneVisitor::visitShader(Shader &shader) {
    activeShader = shader;
    shader.use();
    return 0;
}

int RenderSceneVisitor::visitLight(Light &light) {
    if(!activeShader) return 1;
//    activeShader.value().get().setVec4();
    return 0;
}

int RenderSceneVisitor::visitTransformation(Transformation &transformation) {
    transformationStack.push(transformationStack.top() * transformation.getTransformation());
    return 0;
}

int RenderSceneVisitor::visitMaterial(Material &material) {
    if(!activeShader) return 1;
    if(material.getDiffuseTexture()) {
        activeShader->get().setBool("useTexture", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.getDiffuseTexture()->id);
    } else {
        activeShader->get().setBool("useTexture", false);
    }
    activeShader->get().setVec4("albedo", material.getAlbedo());
    return 0;
}

int RenderSceneVisitor::visitMesh(Mesh &mesh) {
    if(activeShader)
        mesh.render(activeShader->get(), transformationStack.top());
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
