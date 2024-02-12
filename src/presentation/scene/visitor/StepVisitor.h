//
// Created by faliszewskii on 01.02.24.
//

#ifndef OPENGL_SANDBOX_STEPVISITOR_H
#define OPENGL_SANDBOX_STEPVISITOR_H

class RootStep;
class AddFrameBuffer;
class PushTransformation;
class AddCamera;
class AddLight;
class AddMaterial;
class ActivateShader;
class DrawMesh;

class StepVisitor {
public:

    virtual int visitRootStep(RootStep &step) { return 0; };
    virtual int leaveRootStep(RootStep &step) { return 0; };

    virtual int visitAddFrameBuffer(AddFrameBuffer &step) = 0;

    virtual int visitAddLight(AddLight &step) = 0;

    virtual int visitPushTransformation(PushTransformation &step) = 0;
    virtual int leavePushTransformation(PushTransformation &step) { return 0; };

    virtual int visitAddCamera(AddCamera &step) = 0;

    virtual int visitAddMaterial(AddMaterial &step) = 0;

    virtual int visitActivateShader(ActivateShader &step) = 0;

    virtual int visitDrawMesh(DrawMesh &step) = 0;

protected:
    StepVisitor() = default;
};

#endif //OPENGL_SANDBOX_STEPVISITOR_H
