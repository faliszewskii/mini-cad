//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H
#define OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H

#include <array>

struct GregoryPatchGraphVertex {
    int pointId;
    int patchId;
    bool isCornerVertex;
    std::array<int, 4> outgoingBernstein;
};

#endif //OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H
