//
// Created by faliszewskii on 06.05.24.
//

#ifndef OPENGL_SANDBOX_PATCHC2_H
#define OPENGL_SANDBOX_PATCHC2_H

#include <string>
#include "../vertices/PositionVertex.h"
#include "../ids/IdCounter.h"
#include "../entities/mesh/Mesh.h"

class PatchC2 {
    Mesh<PositionVertex> meshGrid;
public:
    int id;
    std::string name;
    bool selected;

    int patchCountX;
    int patchCountY;
    bool wrapped;
    bool drawBezierGrid = false;
    std::vector<std::pair<int, std::reference_wrapper<Point>>> controlPoints;

    PatchC2(const std::vector<PositionVertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<unsigned int> &gridIndices) :
        id(IdCounter::nextId()), name("Patch C2 ("+std::to_string(id)+")"), selected(false),
        mesh(vertices, indices,GL_PATCHES),
        meshGrid(vertices, gridIndices,GL_LINES) {}

    void updatePoint(Point &point, int i) {
        mesh.update({point.position}, i);
        meshGrid.update({point.position}, i);
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("selected", selected);
        glPatchParameteri(GL_PATCH_VERTICES, 16);
        mesh.render(2);
        glLineWidth(1);
    }

    void renderBezierGrid(Shader &shader) {
        if(!drawBezierGrid) return;
        shader.setUniform("selected", false);
        shader.setUniform("model", glm::mat4(1.0f));
        meshGrid.render();
    }

    Mesh<PositionVertex> mesh;
};

#endif //OPENGL_SANDBOX_PATCHC2_H
