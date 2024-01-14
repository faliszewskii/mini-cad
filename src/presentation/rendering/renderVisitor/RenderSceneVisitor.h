//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_RENDERSCENEVISITOR_H
#define OPENGL_SANDBOX_RENDERSCENEVISITOR_H


#include <utility>
#include <stack>
#include <variant>

#include "../../scene/nodes/shader/Shader.h"

class RenderSceneVisitor : public SceneNodeVisitor {
    UniformMap uniformMap;
    std::stack<std::reference_wrapper<Shader>> shaderStack; // TODO Log error if tried to draw something without shader.
    int pointLightCounter; // TODO Not bigger than the maxcap in shader
public:
    explicit RenderSceneVisitor();

    int visitTransformation(Transformation& transformation) override;
    int visitLight(Light &light) override;
    int visitMesh(Mesh& mesh) override;
    int visitCamera(Camera& camera) override;
    int visitShader(Shader& shader) override;
    int visitMaterial(Material &material) override;

    int leaveTransformation(Transformation& transformation) override;
    int leaveLight(Light& light) override;
    int leaveCamera(Camera& camera) override;
    int leaveShader(Shader& shader) override;
    int leaveMaterial(Material &material) override;

};


#endif //OPENGL_SANDBOX_RENDERSCENEVISITOR_H
