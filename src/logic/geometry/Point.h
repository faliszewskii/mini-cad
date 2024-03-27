//
// Created by faliszewskii on 27.02.24.
//

#ifndef OPENGL_SANDBOX_POINT_H
#define OPENGL_SANDBOX_POINT_H

#include "../entities/mesh/Mesh.h"
#include "../vertices/Vertex.h"
#include "../ids/IdCounter.h"

class Point {
public:
    int id;
    std::string name;
    bool selected;
    std::unique_ptr<Mesh<Vertex>> mesh;
    glm::vec3 position;

    glm::vec4 color;
    int size;

    Point(glm::vec3 position = {}, int size = 10, glm::vec4 color = {0,0,0,1}) : selected(false), position(position), mesh(std::make_unique<Mesh<Vertex>>(Mesh<Vertex>(
                {Vertex(glm::vec3(), glm::vec3(), glm::vec2())},
                {},GL_POINTS))), id(IdCounter::nextId()), name("Point (" + std::to_string(id) + ")") ,
                color(color), size(size) {};

    Point(Point &&p) : selected(p.selected), position(p.position), mesh(std::move(p.mesh)), name(p.name), id(p.id) {}

    void render(Shader &shader) const {
        glPointSize(size);
        shader.setUniform("selected", selected);
        shader.setUniform("position", position);
        shader.setUniform("color", color);
        mesh->render();
        glPointSize(1);
    }
};

#endif //OPENGL_SANDBOX_POINT_H
