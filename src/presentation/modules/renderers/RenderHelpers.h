//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_RENDERHELPERS_H
#define OPENGL_SANDBOX_RENDERHELPERS_H

#include "../../scene/nodes/shader/Shader.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../../logic/state/AppState.h"

namespace RenderHelpers {

    inline void setUpCamera(const Camera &camera, const Shader &shader) {
        shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("viewPos", camera.getViewPosition());
    }

    inline void setUpLights(const std::vector<Light> &lights, const Shader &shader) {
        for(int i = 0; i < lights.size(); i++) { // TODO Shader with more lights.
            const Light &light = lights[i];
            shader.setUniform("pointLight.strength", light.getStrength());
            shader.setUniform("pointLight.position", light.getPosition());
            shader.setUniform("pointLight.color", light.getColor());
            shader.setUniform("pointLight.linearAttenuation", light.getLinearAttenuation());
            shader.setUniform("pointLight.constantAttenuation", light.getConstantAttenuation());
            shader.setUniform("pointLight.quadraticAttenuation", light.getQuadraticAttenuation());
        }
    }

    inline void setUpMesh(const Shader &shader, const Mesh &mesh) {
        if(!mesh.material) return;
        auto &material = mesh.material.value().get();
        if (material.getDiffuseTexture()) {
            shader.setUniform("material.useAlbedoTexture", true);
            glActiveTexture(GL_TEXTURE0);
            shader.setUniform("material.textureAlbedo", 0);
            glBindTexture(GL_TEXTURE_2D, material.getDiffuseTexture()->id);
        } else {
            shader.setUniform("material.useAlbedoTexture", false);
        }
        shader.setUniform("material.albedo", material.getAlbedo());
        shader.setUniform("material.shininess", material.getShininess());
    }

    inline void renderMeshes(const TransformTree &transformTree, const Shader &shader) {
        std::stack<glm::mat4> modelStack;
        modelStack.emplace(1.0f);

        std::function<void(const TransformTree &)> lambda;
        lambda = [&](const TransformTree &node){
            modelStack.push(modelStack.top() * node.transform.getTransformation());
            shader.setUniform("model", modelStack.top());
            for(auto &mesh : node.getMeshes()) {
                setUpMesh(shader, *mesh);
                mesh->render();
            }
            for(auto &child : node.getChildren())
                lambda(*child);
            modelStack.pop();
        };

        lambda(transformTree);
    }

    inline void renderMeshesNoMaterial(const TransformTree &transformTree, const Shader &shader) {
        std::stack<glm::mat4> modelStack;
        modelStack.emplace(1.0f);

        std::function<void(const TransformTree &)> lambda;
        lambda = [&](const TransformTree &node){
            modelStack.push(modelStack.top() * node.transform.getTransformation());
            shader.setUniform("model", modelStack.top());
            for(auto &mesh : node.getMeshes())
                mesh->render();
            for(auto &child : node.getChildren())
                lambda(*child);
            modelStack.pop();
        };

        lambda(transformTree);
    }
}

#endif //OPENGL_SANDBOX_RENDERHELPERS_H
