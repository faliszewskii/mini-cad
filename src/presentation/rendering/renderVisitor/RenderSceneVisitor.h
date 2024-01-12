//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_RENDERSCENEVISITOR_H
#define OPENGL_SANDBOX_RENDERSCENEVISITOR_H


#include <utility>
#include <stack>

#include "../../scene/nodes/shader/Shader.h"

class RenderSceneVisitor : public SceneNodeVisitor {
private:
    std::optional<std::reference_wrapper<Shader>> activeShader; // TODO Log error if tried to draw something without shader.
    std::stack<glm::mat4> transformationStack;
public:
    explicit RenderSceneVisitor();

    int visitTransformation(Transformation& transformation) override;
    int visitMesh(Mesh& mesh) override;
    int visitCamera(Camera& camera) override;
    int visitShader(Shader& shader) override;

    int leaveTransformation(Transformation& transformation) override;
};


#endif //OPENGL_SANDBOX_RENDERSCENEVISITOR_H
