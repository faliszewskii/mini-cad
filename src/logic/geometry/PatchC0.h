//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_PATCHC0_H
#define OPENGL_SANDBOX_PATCHC0_H

#include <string>
#include "Point.h"
#include "../ids/IdCounter.h"
#include "../vertices/PositionVertex.h"

class PatchC0 {
    Mesh<PositionVertex> mesh;
    Mesh<PositionVertex> meshGrid;
public:
    int id;
    std::string name;
    bool selected;

    bool drawBezierGrid = false;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    PatchC0(const std::vector<PositionVertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<unsigned int> &gridIndices) :
        id(IdCounter::nextId()), name("Patch C0 ("+std::to_string(id)+")"), selected(false),
        mesh(vertices,indices,GL_PATCHES),
        meshGrid(vertices, gridIndices,GL_LINES) {}

    void updatePoint(Point &point, int i) {
        mesh.update({point.position}, i);
        meshGrid.update({point.position}, i);
    }

    void render(Shader &shader) {
        glLineWidth(3);
        shader.setUniform("selected", selected);
        glPatchParameteri(GL_PATCH_VERTICES, 16);
        mesh.render(2);
        glLineWidth(1);
    }

    void renderBezierGrid(Shader &shader) {
        if(!drawBezierGrid) return;
        glLineWidth(2);
        shader.setUniform("selected", false);
        shader.setUniform("model", glm::mat4(1.0f));
        meshGrid.render();
        glLineWidth(1);
    }
};

#endif //OPENGL_SANDBOX_PATCHC0_H
