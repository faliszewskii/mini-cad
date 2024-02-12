//
// Created by faliszewskii on 01.02.24.
//

#ifndef OPENGL_SANDBOX_EXECUTESTEPVISITOR_H
#define OPENGL_SANDBOX_EXECUTESTEPVISITOR_H


#include <set>
#include "../../scene/visitor/StepVisitor.h"
#include "../../scene/nodes/shader/Shader.h"

class ExecuteStepVisitor : public StepVisitor {
    UniformMap uniformMap;
    constexpr static auto cmp = [](Shader& a, Shader& b) { return std::addressof(a) < std::addressof(b); };
    std::set<std::reference_wrapper<Shader>, decltype(cmp)> activeShaders;
public:
    int visitRootStep(RootStep &step) override;

    int visitAddFrameBuffer(AddFrameBuffer &step) override;

    int visitPushTransformation(PushTransformation &step) override;
    int leavePushTransformation(PushTransformation &step) override;

    int visitAddCamera(AddCamera &step) override;

    int visitAddLight(AddLight &step) override;

    int visitAddMaterial(AddMaterial &step) override;

    int visitActivateShader(ActivateShader &step) override;

    int visitDrawMesh(DrawMesh &step) override;
};


#endif //OPENGL_SANDBOX_EXECUTESTEPVISITOR_H
