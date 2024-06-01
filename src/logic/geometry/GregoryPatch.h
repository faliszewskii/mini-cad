//
// Created by faliszewskii on 30.05.24.
//

#ifndef OPENGL_SANDBOX_GREGORYPATCH_H
#define OPENGL_SANDBOX_GREGORYPATCH_H


#include <string>
#include <utility>
#include "../vertices/PositionVertex.h"
#include "../entities/mesh/Mesh.h"
#include "Point.h"
#include "../math/graph/GregoryPatchGraphVertex.h"
#include "../events/create/CreatePointEvent.h"
#include "../creator/GregoryPatchCreator.h"

struct AppState;
class GregoryPatch {
    Mesh<PositionVertex> mesh;
    Mesh<PositionVertex> vectorMesh;
public:
    int id;
    std::string name;
    bool selected;
    AppState &appState;

    int patchGridWidth = 3;
    int patchGridLength = 3;
    bool drawVectors = false;

    // References to points that affect the patch
    std::vector<std::array<GregoryPatchGraphVertex, 4>> patchSides;

    // Position of the control points. To be updated using creator if any of the points above change (or the patches)
    std::vector<std::array<glm::vec3, 7>> p3is; // P3i points for each side.
    std::vector<glm::vec3> p2is; // P2i points for each side.
    std::vector<glm::vec3> p1is; // P1i points for each side.
    glm::vec3 p0; // P0 point. The middle of the patch.
    std::vector<std::array<glm::vec3, 4>> fiSide;
    std::vector<std::array<glm::vec3, 2>> fiMiddle;
    std::vector<std::array<glm::vec3, 2>> fiLast;

    GregoryPatch(
            AppState &appState,
            std::vector<std::array<GregoryPatchGraphVertex, 4>> patchSides,
            std::vector<std::array<glm::vec3, 7>> p3is,
            std::vector<glm::vec3> p2is,
            std::vector<glm::vec3> p1is,
            glm::vec3 p0,
            std::vector<std::array<glm::vec3, 4>> fiSide,
            std::vector<std::array<glm::vec3, 2>> fiMiddle,
            std::vector<std::array<glm::vec3, 2>> fiLast
            ) :
            appState(appState),
            id(IdCounter::nextId()), name("Gregory Patch ("+std::to_string(id)+")"), selected(false),
            mesh(Mesh<PositionVertex>({}, {}, GL_PATCHES)),
            vectorMesh(Mesh<PositionVertex>({}, {}, GL_LINES)),
            patchSides(std::move(patchSides)), p3is(std::move(p3is)), p2is(std::move(p2is)), p1is(std::move(p1is)),
            p0(p0), fiSide(std::move(fiSide)), fiMiddle(std::move(fiMiddle)), fiLast(std::move(fiLast)) {
        updateMesh();
    }

    void updateMesh() {
        std::vector<PositionVertex> vertices;
        std::vector<PositionVertex> vectors;

        for(int i = 0; i < patchSides.size(); i++) {
            int side = i;
            int sideNext = (i+1)%patchSides.size();

            vertices.emplace_back(p3is[side][3]);
            vertices.emplace_back(p3is[side][4]);
            vertices.emplace_back(p3is[side][5]);
            vertices.emplace_back(p3is[side][6]); // p3is[sideNext][0]
            vertices.emplace_back(p3is[sideNext][1]);
            vertices.emplace_back(p3is[sideNext][2]);
            vertices.emplace_back(p3is[sideNext][3]);

            vertices.emplace_back(p2is[sideNext]);
            vertices.emplace_back(p1is[sideNext]);
            vertices.emplace_back(p0);
            vertices.emplace_back(p1is[side]);
            vertices.emplace_back(p2is[side]);

            vertices.emplace_back(fiSide[side][2]);
            vertices.emplace_back(fiSide[side][3]);
            vertices.emplace_back(fiSide[sideNext][0]);
            vertices.emplace_back(fiSide[sideNext][1]);
            vertices.emplace_back(fiMiddle[sideNext][0]); // The same as previous // TODO Maybe bad
            vertices.emplace_back(fiLast[sideNext][0]); // TODO BAD FOR SURE
            vertices.emplace_back(fiLast[side][1]);  // TODO BAD FOR SURE
            vertices.emplace_back(fiMiddle[side][1]); // Same as next // TODO Maybe bad

            // Vectors

            for(int i = 0; i < p3is[side].size()-1; i++) {
                vectors.emplace_back(p3is[side][i]);
                vectors.emplace_back(p3is[side][i+1]);
            }

            vectors.emplace_back(p3is[side][3]);
            vectors.emplace_back(p2is[side]);

            vectors.emplace_back(p2is[side]);
            vectors.emplace_back(p1is[side]);

            vectors.emplace_back(p1is[side]);
            vectors.emplace_back(p0);

            auto s = {1, 2, 4, 5};
            for(int j = 0; j < s.size(); j++) {
                vectors.emplace_back(p3is[side][s.begin()[j]]);
                vectors.emplace_back(fiSide[side][j]);
            }
            for(int j = 0; j < 2 ;j++) {
                vectors.emplace_back(p2is[side]);
                vectors.emplace_back(fiMiddle[side][j]);

                vectors.emplace_back(p1is[side]);
                vectors.emplace_back(fiLast[side][j]);
            }
        }

        mesh.update(std::move(vertices), {});
        vectorMesh.update(std::move(vectors), {});
    }

    void recalculate(GregoryPatchCreator &creator) {
        creator.setHole(patchSides);
        creator.fillHole(appState);
        p3is = creator.p3is;
        p2is = creator.p2is;
        p1is = creator.p1is;
        p0 = creator.p0;
        fiSide = creator.fiSide;
        fiMiddle = creator.fiMiddle;
        fiLast = creator.fiLast;
        updateMesh();
    }

    void render(Shader &shader) {
        glLineWidth(2);
        shader.setUniform("gridCountWidth", patchGridWidth);
        shader.setUniform("gridCountLength", patchGridLength);
        shader.setUniform("selected", selected);
        glPatchParameteri(GL_PATCH_VERTICES, 20);
        mesh.render(2);
        glLineWidth(1);
    }

    void renderVectors(Shader &shader) {
        if(!drawVectors) return;
        glLineWidth(2);
        shader.setUniform("color", glm::vec4(0, 0.5, 0.5, 1));
        shader.setUniform("model", glm::mat4(1.0f));
        vectorMesh.render();
        glLineWidth(1);
    }


};

#endif //OPENGL_SANDBOX_GREGORYPATCH_H
