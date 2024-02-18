//
// Created by faliszewskii on 18.02.24.
//


#include "ParametrisedMeshGenerator.h"

void ParametrisedMeshGenerator::generate() {
    // TODO Add rules for connecting edges (if closed) and in what direction to connect them.
    // TODO Add indices.
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float uStep = 1.f / (float)(uCount-1) * (uMax - uMin);
    float vStep = 1.f / (float)(vCount-1) * (vMax - vMin);
    for (int i = 0; i < uCount; i++) { // +1 or connection rules in indices
        float u = (float)i * uStep + uMin;
        for (int j = 0; j < vCount; j++) {
            float v = (float)j * vStep + vMin;
            auto pos = glm::vec3(x(u, v), y(u, v), z(u, v));
            auto U = glm::vec3(xdu(u, v), ydu(u, v), zdu(u, v));
            auto V = glm::vec3(xdv(u, v), ydv(u, v), zdv(u, v));
            vertices.emplace_back(pos, glm::normalize(glm::cross(U,V)));
        }
    }
    for (int i = 0; i < uCount-1; i++) {
        auto i_next = (i + 1);
        for (int j = 0; j < vCount-1; j++) {
            auto j_next = (j + 1);
            auto i0 = i * vCount + j;
            auto i1 = i * vCount + j_next;
            auto i2 = i_next * vCount + j_next;
            auto i3 = i_next * vCount + j;

            MeshGeneratorHelpers::addQuad(indices, i0, i1, i2, i3);
        }
    }
    target.update(std::move(vertices), std::move( std::optional<std::vector<unsigned int>>(indices)));
}

MeshGenerator::ParameterMap ParametrisedMeshGenerator::getParameters() {
    return MeshGenerator::ParameterMap {
            {"U Count", uCount},
            {"V Count", vCount},
            {"U Min", uMin},
            {"U Max", uMax},
            {"V Min", vMin},
            {"V Max", vMax},
    };
}
