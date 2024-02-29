//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_RENDERHELPERS_H
#define OPENGL_SANDBOX_RENDERHELPERS_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/geometry/Point.h"

namespace RenderHelpers {

    inline void setUpCamera(const Camera &camera, const std::vector<std::reference_wrapper<Shader>> &shaders) {
        for(auto &shaderRef : shaders) {
            auto &shader = shaderRef.get();
            shader.use();
            shader.setUniform("projection", camera.getProjectionMatrix());
            shader.setUniform("view", camera.getViewMatrix());
            shader.setUniform("viewPos", camera.getViewPosition());
        }
    }

    inline void setUpLights(const std::vector<Light> &lights, const std::vector<std::reference_wrapper<Shader>> &shaders) {
        for(auto &shaderRef : shaders) {
            auto &shader = shaderRef.get();
            shader.use();
            for (int i = 0; i < lights.size(); i++) { // TODO Shader with more lights.
                const Light &light = lights[i];
                shader.setUniform("pointLight.strength", light.getStrength());
                shader.setUniform("pointLight.position", light.getPosition());
                shader.setUniform("pointLight.color", light.getColor());
                shader.setUniform("pointLight.linearAttenuation", light.getLinearAttenuation());
                shader.setUniform("pointLight.constantAttenuation", light.getConstantAttenuation());
                shader.setUniform("pointLight.quadraticAttenuation", light.getQuadraticAttenuation());
            }
        }
    }

    template<typename TVertex>
    inline void setUpMesh(const Mesh<TVertex> &mesh, const std::vector<std::reference_wrapper<Shader>> &shaders) {
        if(!mesh.material) return;
        auto &material = mesh.material.value().get();
        for(auto &shaderRef : shaders) {
            auto &shader = shaderRef.get();
            shader.use();
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
    }

    inline void renderMeshes(TransformTree &transformTree, const std::vector<std::reference_wrapper<Shader>> &shaders, auto visitor) {
        std::stack<glm::mat4> modelStack;
        modelStack.emplace(1.0f);

        std::function<void(TransformTree &)> lambda;
        lambda = [&](TransformTree &node){
            modelStack.push(modelStack.top() * node.transform.getTransformation());
            for(auto &shaderRef : shaders) {
                const auto &shader = shaderRef.get();
                shader.use();
                shader.setUniform("model", modelStack.top());
            }
            for(auto &entity : node.getEntities()) {
                std::visit(visitor, entity);
            }
            for(auto &child : node.getChildren())
                lambda(*child);
            modelStack.pop();
        };

        lambda(transformTree);
    }
}

#endif //OPENGL_SANDBOX_RENDERHELPERS_H
