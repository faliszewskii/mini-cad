//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_RENDERSCENEVISITOR_H
#define OPENGL_SANDBOX_RENDERSCENEVISITOR_H


#include <utility>
#include <stack>

#include "../shader/shader.h"

class RenderSceneVisitor : public SceneNodeVisitor {
private:
    Shader& activeShader;
    std::stack<glm::mat4> transformationStack;
public:
    explicit RenderSceneVisitor(Shader& activeShader);

    int visitTransformation(Transformation& transformation) override;
    int visitMesh(Mesh& mesh) override;
    int visitPointLight(PointLight& pointLight) override;
    int visitCamera(Camera& camera) override;

    int leaveTransformation(Transformation& transformation) override;
};


#endif //OPENGL_SANDBOX_RENDERSCENEVISITOR_H
