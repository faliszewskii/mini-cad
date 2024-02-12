//
// Created by faliszewskii on 02.02.24.
//

#ifndef OPENGL_SANDBOX_STEPPARAMETERSVISITOR_H
#define OPENGL_SANDBOX_STEPPARAMETERSVISITOR_H


#include "../../scene/visitor/StepVisitor.h"
#include "../../scene/steps/RenderingStep.h"

class StepParametersVisitor : public StepVisitor {
public:
    int visitAddFrameBuffer(AddFrameBuffer &step) override;

    int visitPushTransformation(PushTransformation &step) override;

    int visitAddCamera(AddCamera &step) override;

    int visitAddLight(AddLight &step) override;

    int visitAddMaterial(AddMaterial &step) override;

    int visitActivateShader(ActivateShader &step) override;

    int visitDrawMesh(DrawMesh &step) override;

    void drawHeader(RenderingStep &step);
};


#endif //OPENGL_SANDBOX_STEPPARAMETERSVISITOR_H
