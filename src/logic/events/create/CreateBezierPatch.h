//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_CREATEBEZIERPATCH_H
#define OPENGL_SANDBOX_CREATEBEZIERPATCH_H

#include "../../creator/BezierPatchCreatorParams.h"

struct CreateBezierPatch {
    int patchCountX;
    int patchCountY;
    bool wrapped;
    std::vector<unsigned int> patchIndices;
    std::vector<unsigned int> gridIndices;
    std::vector<int> controlPoints;
    bool C2;
};

#endif //OPENGL_SANDBOX_CREATEBEZIERPATCH_H
