//
// Created by faliszewskii on 07.01.24.
//

#include <glm/gtx/string_cast.hpp>
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/camera/Camera.h"
#include "../../scene/nodes/light/Light.h"
#include "RenderSceneVisitor.h"

RenderSceneVisitor::RenderSceneVisitor() : pointLightCounter(0) {}

int RenderSceneVisitor::visitShader(Shader &shader) {
    shaderStack.emplace(shader);
    return 0;
}

int RenderSceneVisitor::leaveShader(Shader &shader) {
    shaderStack.pop();
    return 0;
}

int RenderSceneVisitor::visitLight(Light &light) {
    // TODO Here property visitor on chosen light type.
    // TODO pointLightCounter
    uniformMap["pointLight.strength"].emplace(light.strength); // TODO Research light attenuation.
    uniformMap["pointLight.position"].emplace(light.pointLightProperty->getPosition());
    uniformMap["pointLight.color"].emplace(light.color);
    return 0;
}

int RenderSceneVisitor::leaveLight(Light &light) {
    uniformMap["pointLight.color"].pop();
    uniformMap["pointLight.position"].pop();
    uniformMap["pointLight.strength"].pop();
    return 0;
}

int RenderSceneVisitor::visitTransformation(Transformation &transformation) {
    if(uniformMap.contains("model")) {
        auto& globalTransform = std::get<glm::mat4>(uniformMap["model"].top());
        uniformMap["model"].emplace(globalTransform * transformation.getTransformation());
    } else
        uniformMap["model"].emplace(transformation.getTransformation());
    return 0;
}

int RenderSceneVisitor::leaveTransformation(Transformation &transformation) {
    uniformMap["model"].pop();
    return 0;
}

int RenderSceneVisitor::visitMaterial(Material &material) {
    if(material.getDiffuseTexture()) {
        uniformMap["material.useAlbedoTexture"].emplace(true);
        glActiveTexture(GL_TEXTURE0);
        uniformMap["material.textureAlbedo"].emplace(0);
        glBindTexture(GL_TEXTURE_2D, material.getDiffuseTexture()->id);
    } else {
        uniformMap["material.useAlbedoTexture"].emplace(false);
    }
    uniformMap["material.albedo"].emplace(material.getAlbedo());
    uniformMap["material.shininess"].emplace(material.getShininess());
    return 0;
}

int RenderSceneVisitor::leaveMaterial(Material &material) {
    uniformMap["material.useAlbedoTexture"].pop();
    uniformMap["material.shininess"].pop();
    uniformMap["material.albedo"].pop();
    return 0;
}

// settings // TODO decide what to do about screen width and perspective in camera.
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int RenderSceneVisitor::visitCamera(Camera &camera) {
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)camera.screenWidth / (float)camera.screenHeight, 0.1f, 100.0f); // TODO configurable
    glm::mat4 view = camera.getViewMatrix();
    uniformMap["projection"].emplace(projection);
    uniformMap["view"].emplace(view);
    uniformMap["viewPos"].emplace(camera.getViewPosition());
    return 0;
}

int RenderSceneVisitor::leaveCamera(Camera &camera) {
    uniformMap["viewPos"].pop();
    uniformMap["view"].pop();
    uniformMap["projection"].pop();
    return 0;
}

int RenderSceneVisitor::visitMesh(Mesh &mesh) {
    if(shaderStack.empty()) return 1;
    shaderStack.top().get().setUniforms(uniformMap);
    mesh.render();
    return 0;
}
