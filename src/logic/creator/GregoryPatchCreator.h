//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GREGORYPATCHCREATOR_H
#define OPENGL_SANDBOX_GREGORYPATCHCREATOR_H


#include <vector>
#include <map>
#include "../math/graph/Graph.h"
#include "../geometry/PatchC0.h"
#include "../math/graph/GregoryPatchGraphVertex.h"

struct AppState;

class GregoryPatchCreator {
    int vId{};
    std::vector<std::vector<int>> holePoints;
    std::vector<std::vector<GregoryPatchGraphVertex>> holes;

public:

    // Generated after fillHole();
    std::vector<std::array<GregoryPatchGraphVertex, 4>> patchSides;
    std::vector<std::array<glm::vec3, 7>> p3is; // P3i points for each side.
    std::vector<glm::vec3> p2is; // P2i points for each side.
    std::vector<glm::vec3> p1is; // P1i points for each side.
    glm::vec3 p0; // P0 point. The middle of the patch.
    std::vector<std::array<glm::vec3, 4>> fiSide;
    std::vector<std::array<glm::vec3, 2>> fiMiddle;
    std::vector<std::array<glm::vec3, 2>> fiLast;

    void reset();
    std::vector<std::vector<int>> getHolePoints();

    void findHoles(AppState &appState, const std::vector<int>& bezierPatchIds, int size);

    void fillHole(AppState &state);

    void prepareHole(int holeId);
    void setHole(std::vector<std::array<GregoryPatchGraphVertex, 4>> hole);

private:
    void addPatchToGraph(AppState &appState, Graph<GregoryPatchGraphVertex> &graph,
                         std::map<int, int> &cornerVerticesMap,
                         const PatchC0 &patch, int size, const std::function<int(int,int)>& small,
                         const std::function<int(int,int)>& helper);

    static std::pair<std::array<glm::vec3, 4>, std::array<glm::vec3, 4>> divideDeCasteljau(std::array<glm::vec3, 4> ctrlPoints, float t);

    glm::vec3 deCasteljau2(std::array<glm::vec3, 3> ctrlPoints, float t);
};


#endif //OPENGL_SANDBOX_GREGORYPATCHCREATOR_H
