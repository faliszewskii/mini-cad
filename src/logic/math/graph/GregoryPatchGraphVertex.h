//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H
#define OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H

#include <array>

struct GregoryPatchGraphVertex {
    int id;
    int pointId;
    int patchId;
    bool isCornerVertex;
    std::array<int, 4> outgoingBernstein;

    friend bool operator==(const GregoryPatchGraphVertex &e, const GregoryPatchGraphVertex &o) {
        return e.id == o.id;
    }
};

#endif //OPENGL_SANDBOX_GREGORYPATCHGRAPHVERTEX_H
