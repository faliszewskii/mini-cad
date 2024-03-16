//
// Created by faliszewskii on 16.03.24.
//

#ifndef OPENGL_SANDBOX_SPLINEVERTEX_H
#define OPENGL_SANDBOX_SPLINEVERTEX_H

#include <glm/vec3.hpp>
#include <GL/gl.h>
#include <vector>

struct SplineVertex{
    glm::vec3 position;
    float knot;

    inline static std::vector<int> getSizes() { return {3, 1}; }
    inline static std::vector<int> getTypes() { return {GL_FLOAT, GL_FLOAT}; }
    inline static std::vector<int> getOffsets() { return {0, offsetof(SplineVertex, knot)}; }
};

#endif //OPENGL_SANDBOX_SPLINEVERTEX_H
