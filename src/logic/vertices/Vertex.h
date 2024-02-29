//
// Created by faliszewskii on 29.02.24.
//

#ifndef OPENGL_SANDBOX_VERTEX_H
#define OPENGL_SANDBOX_VERTEX_H

#include <glm/vec3.hpp>
#include <GL/gl.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    inline static std::vector<int> getSizes() { return {3, 3, 2}; }
    inline static std::vector<int> getTypes() { return {GL_FLOAT, GL_FLOAT, GL_FLOAT}; }
    inline static std::vector<int> getOffsets() { return {0, offsetof(Vertex, normal), offsetof(Vertex, texCoords)}; }
};

#endif //OPENGL_SANDBOX_VERTEX_H
