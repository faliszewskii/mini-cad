//
// Created by faliszewskii on 01.02.24.
//

#include "ExecuteStepVisitor.h"

#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/steps/RenderingStep.h"

int ExecuteStepVisitor::visitRootStep(RootStep &step) {
    uniformMap["model"].emplace(glm::mat4(1.0f));
    return 0;
}

int ExecuteStepVisitor::visitAddFrameBuffer(AddFrameBuffer &step) {
    return 0;
}

int ExecuteStepVisitor::visitPushTransformation(PushTransformation &step) {
    if (!uniformMap["model"].empty()) {
        auto &globalTransform = std::get<glm::mat4>(uniformMap["model"].top());
        uniformMap["model"].emplace(globalTransform * step.transformation.getTransformation());
    } else
        uniformMap["model"].emplace(step.transformation.getTransformation());
    return 0;
}

int ExecuteStepVisitor::leavePushTransformation(PushTransformation &step) {
    uniformMap["model"].pop();
    return 0;
}

int ExecuteStepVisitor::visitAddCamera(AddCamera &step) {
    glm::mat4 projection = step.camera.getProjectionMatrix();
    glm::mat4 view = step.camera.getViewMatrix();
    uniformMap["projection"].emplace(projection);
    uniformMap["view"].emplace(view);
    uniformMap["viewPos"].emplace(step.camera.getViewPosition());
    return 0;
}

int ExecuteStepVisitor::visitAddLight(AddLight &step) {
    // TODO Here property visitor on chosen light type.
    // TODO pointLightCounter
    uniformMap["pointLight.strength"].emplace(step.light.getStrength()); // TODO Research light attenuation.
    uniformMap["pointLight.position"].emplace(step.light.getPosition());
    uniformMap["pointLight.color"].emplace(step.light.getColor());
    uniformMap["pointLight.linearAttenuation"].emplace(step.light.getLinearAttenuation());
    uniformMap["pointLight.constantAttenuation"].emplace(step.light.getConstantAttenuation());
    uniformMap["pointLight.quadraticAttenuation"].emplace(step.light.getQuadraticAttenuation());
    return 0;
}

int ExecuteStepVisitor::visitAddMaterial(AddMaterial &step) {
    if (step.material.getDiffuseTexture()) {
        uniformMap["material.useAlbedoTexture"].emplace(true);
        glActiveTexture(GL_TEXTURE0);
        uniformMap["material.textureAlbedo"].emplace(0);
        glBindTexture(GL_TEXTURE_2D, step.material.getDiffuseTexture()->id);
    } else {
        uniformMap["material.useAlbedoTexture"].emplace(false);
    }
    uniformMap["material.albedo"].emplace(step.material.getAlbedo());
    uniformMap["material.shininess"].emplace(step.material.getShininess());
    return 0;
}

int ExecuteStepVisitor::visitActivateShader(ActivateShader &step) {
    if (step.shader.active) activeShaders.emplace(step.shader);
    return 0;
}

int ExecuteStepVisitor::visitDrawMesh(DrawMesh &step) {
    for(Shader& shader : activeShaders) {
        shader.setUniforms(uniformMap);
        step.mesh.render();
    }
    return 0;
}
