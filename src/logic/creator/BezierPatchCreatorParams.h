//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_BEZIERPATCHCREATORPARAMS_H
#define OPENGL_SANDBOX_BEZIERPATCHCREATORPARAMS_H

struct BezierPatchCreatorParams {
    int patchCountWidth;
    int patchCountLength;

    // Plane
    float planeWidth;
    float planeLength;
    // Cylinder
    float cylinderHeight;
    float cylinderRadius;

    bool wrapped;
    bool C2;
    bool dance;
};

#endif //OPENGL_SANDBOX_BEZIERPATCHCREATORPARAMS_H
