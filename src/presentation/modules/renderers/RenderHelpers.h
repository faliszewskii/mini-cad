//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_RENDERHELPERS_H
#define OPENGL_SANDBOX_RENDERHELPERS_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/geometry/Point.h"

namespace RenderHelpers {

    inline void setUpCamera(const Camera &camera, const Shader &shader) {
        shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("viewPos", camera.getViewPosition());
    }
//
//    inline void renderMeshes(TransformTree &transformTree, const std::vector<std::reference_wrapper<Shader>> &shaders, auto visitor) {
//        std::stack<glm::mat4> modelStack;
//        modelStack.emplace(1.0f);
//
//        std::function<void(TransformTree &)> lambda;
//        lambda = [&](TransformTree &node){
//            modelStack.push(modelStack.top() * node.transform.getTransformation());
//            for(auto &shaderRef : shaders) {
//                const auto &shader = shaderRef.get();
//                shader.use();
//                shader.setUniform("model", modelStack.top());
//            }
//            for(auto &entity : node.getEntities()) {
//                std::visit(visitor, entity);
//            }
//            for(auto &child : node.getChildren())
//                lambda(*child);
//            modelStack.pop();
//        };
//
//        lambda(transformTree);
//    }
}

#endif //OPENGL_SANDBOX_RENDERHELPERS_H
