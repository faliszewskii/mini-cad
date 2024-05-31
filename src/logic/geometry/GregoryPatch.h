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
//#include "../state/AppState.h"
struct AppState;
class GregoryPatch {
    Mesh<PositionVertex> mesh;
public:
    int id;
    std::string name;
    bool selected;

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
            id(IdCounter::nextId()), name("Gregory Patch ("+std::to_string(id)+")"), selected(false),
            mesh(Mesh<PositionVertex>({}, {}, GL_PATCHES)),
            patchSides(std::move(patchSides)), p3is(std::move(p3is)), p2is(std::move(p2is)), p1is(std::move(p1is)),
            p0(p0), fiSide(std::move(fiSide)), fiMiddle(std::move(fiMiddle)), fiLast(std::move(fiLast)) {
        updateMesh(/*appState*/);
    }

    void updateMesh(/*AppState &appState*/) {
        std::vector<PositionVertex> vertices;

        std::stringstream ss;
        ss << "Creating Gregory mesh:\n";

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

            /*for(int k = 0; k < 20; k++) {
                appState.eventPublisher.publish(CreatePointEvent{vertices[i*20 + k].position + glm::vec3(0,*//*(i+1)*0.05f*//*0,0)});
                appState.pointSet[appState.lastIdCreated]->color = glm::vec4(
                        i==0? 0.2 + ((float)k/20)*0.8f : 0,
                        i==1? 0.2 + ((float)k/20)*0.8f : 0,
                        i==2? 0.2 + ((float)k/20)*0.8f : 0,
                        1
                        );
            }*/
        }

        mesh.update(std::move(vertices), {});
    }

    void render(Shader &shader) {
        shader.setUniform("selected", selected);
        glPatchParameteri(GL_PATCH_VERTICES, 20);
        mesh.render(2);
    }
};

#endif //OPENGL_SANDBOX_GREGORYPATCH_H
