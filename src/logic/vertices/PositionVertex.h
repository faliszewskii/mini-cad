//
// Created by faliszewskii on 09.03.24.
//

#ifndef OPENGL_SANDBOX_POSITIONVERTEX_H
#define OPENGL_SANDBOX_POSITIONVERTEX_H

#include <glm/vec3.hpp>
#include "../glad_glfw.h"
#include <vector>

struct PositionVertex{
    glm::vec3 position;

    inline static std::vector<int> getSizes() { return {3}; }
    inline static std::vector<int> getTypes() { return {GL_FLOAT}; }
    inline static std::vector<int> getOffsets() { return {0}; }
};

#endif //OPENGL_SANDBOX_POSITIONVERTEX_H
