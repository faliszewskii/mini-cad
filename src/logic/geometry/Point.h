//
// Created by faliszewskii on 27.02.24.
//

#ifndef OPENGL_SANDBOX_POINT_H
#define OPENGL_SANDBOX_POINT_H

#include "../entities/mesh/Mesh.h"

class Point {
public:
    std::unique_ptr<Mesh<Vertex>> mesh;
    glm::vec3 position;

    Point(glm::vec3 position = {}) : position(position), mesh(std::make_unique<Mesh<Vertex>>(Mesh<Vertex>("Point",
                std::vector<Vertex>{Vertex(glm::vec3(), glm::vec3(), glm::vec2())},
                {},{},GL_POINTS))) {};

    void render(Shader &shader) const {
        glPointSize(15);
        shader.setUniform("position", position);
        mesh->render();
        glPointSize(1);
    }
};

#endif //OPENGL_SANDBOX_POINT_H
