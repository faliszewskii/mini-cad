//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_RENDERSCENEVISITOR_H
#define OPENGL_SANDBOX_RENDERSCENEVISITOR_H


#include <utility>
#include <stack>
#include <variant>
#include <set>

#include "../../scene/nodes/shader/Shader.h"

class RenderSceneVisitor : public SceneNodeVisitor {
    UniformMapOld uniformMap;
    constexpr static auto cmp = [](Shader& a, Shader& b) { return std::addressof(a) < std::addressof(b); };
    std::set<std::reference_wrapper<Shader>, decltype(cmp)> activeShaders;
    int pointLightCounter; // TODO Not bigger than the maxcap in shader
public:
    explicit RenderSceneVisitor();

    int visitFrameBuffer(FrameBuffer &frameBuffer) override;
    int visitTransformation(Transformation &transformation) override;
    int visitLight(Light &light) override;
    int visitMesh(Mesh &mesh) override;
    int visitCamera(Camera &camera) override;
    int visitShader(Shader &shader) override;
    int visitMaterial(Material &material) override;

    int leaveFrameBuffer(FrameBuffer &frameBuffer) override;
    int leaveTransformation(Transformation &transformation) override;
    int leaveLight(Light &light) override;
    int leaveCamera(Camera &camera) override;
    int leaveShader(Shader &shader) override;
    int leaveMaterial(Material &material) override;
};


#endif //OPENGL_SANDBOX_RENDERSCENEVISITOR_H
