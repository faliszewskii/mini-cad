//
// Created by faliszewskii on 29.05.24.
//

#include "GregoryPatchCreator.h"

#include <utility>
#include "../state/AppState.h"

void  GregoryPatchCreator::findHoles(AppState &appState, const std::vector<int>& bezierPatchIds, int size) {
    holePoints.clear();
    holes.clear();
    vId = 0;
    Graph<GregoryPatchGraphVertex> graph;
    std::map<int, int> cornerVerticesMap; // point id -> graph vertex id // Only for corner vertices.

    for(auto &id : bezierPatchIds) {
        auto &patch = *appState.patchC0Set[id];

        auto getId = [](int i, int j){return 3 * i + j;};
        int fullRow = 3 * patch.patchCountX + (patch.wrapped ? 0 : 1);
        // TOP PATCH ROW
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
            [&](int i, int j){return patch.wrapped? (i==patch.patchCountX-1 && j == 3 ? 0 : getId(i,j)) : getId(i,j);},
            [&](int i, int j){ return (patch.wrapped && i==patch.patchCountX-1 && j == 3) ? (fullRow) : getId(i,j) + (fullRow); }
        );

        // BOTTOM PATCH ROW
        int offset = patch.controlPoints.size() - (fullRow);
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
            [&](int i, int j){return (patch.wrapped && i==patch.patchCountX-1 && j == 3) ? offset + 0 : offset + getId(i,j);},
            [&](int i, int j){ return (patch.wrapped && i==patch.patchCountX-1 && j == 3) ? offset - (fullRow) : offset + getId(i,j) - (fullRow); }
        );

       if(!patch.wrapped) { // Only bottoms when a cylinder.
           int xSize = 3 * patch.patchCountX + 1;
           // LEFT PATCH COLUMN
           addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
               [&](int i, int j){return xSize * getId(i,j);},
               [&](int i, int j){return xSize * getId(i,j) + 1;}
           );

           // RIGHT PATCH COLUMN
           addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
               [&](int i, int j){return xSize * getId(i,j) + (xSize-1);},
               [&](int i, int j){return xSize * getId(i,j) + (xSize-1) - 1;}
           );
       }
    }
    std::stringstream ss;
    ss << "DFS: ";
    for(auto &v : graph.dfs()) {
        ss << v.pointId << "(" << (v.isCornerVertex? "C":"N") << ")" << ", ";
    }
    appState.logger.logDebug(ss.str());

    holes = graph.findCycles(size * 5);

    std::ranges::transform(holes, std::back_inserter(holePoints), [](auto &el){
        auto v1 = el | std::views::filter([](auto &inner) { return !inner.isCornerVertex; });
        std::vector<int> v;
        std::ranges::transform(v1, std::back_inserter(v), [&](auto &inner) { return inner.pointId; });

        std::unordered_set<int> s;
        auto last = std::remove_if(v.begin(), v.end(), [&](int n) {
            return (s.find(n) == s.end()) ? (s.insert(n), false) : true;
        });
        v.erase(last, v.end());

        return v;
    });
}

void GregoryPatchCreator::addPatchToGraph(AppState &appState, Graph<GregoryPatchGraphVertex> &graph,
                                          std::map<int, int> &cornerVerticesMap,
                                          const PatchC0 &patch, int size, const std::function<int(int,int)>& small,
                                          const std::function<int(int,int)>& helper) {
    for(int i = 0; i < size; i++) {
        std::array<int, 4> smallVertices{};
        for(int j = 0; j < 4; j++) {
            int smallVertex = patch.controlPoints[small(i, j)].first; // Here
            int helperVertex = patch.controlPoints[helper(i, j)].first;
            smallVertices[j] = graph.addVertex({vId++, smallVertex, patch.id, false, helperVertex});
        }
        for(int j = 0; j < 3; j++) {
            graph.addEdge(smallVertices[j], smallVertices[j + 1]);
            graph.addEdge(smallVertices[j + 1], smallVertices[j]);
        }
        int firstPoint = patch.controlPoints[small(i,0)].first; // Here
        int lastPoint = patch.controlPoints[small(i,3)].first; // Here
        if(!cornerVerticesMap.contains(firstPoint)) {
            int corner = graph.addVertex({vId++, firstPoint, patch.id, true});
            cornerVerticesMap.insert({firstPoint, corner});
        }
        if(!cornerVerticesMap.contains(lastPoint)) {
            int corner = graph.addVertex({vId++, lastPoint, patch.id, true});
            cornerVerticesMap.insert({lastPoint, corner});
        }

        graph.addEdge(cornerVerticesMap[firstPoint], smallVertices[0]);
        graph.addEdge(smallVertices[0], cornerVerticesMap[firstPoint]);

        graph.addEdge(cornerVerticesMap[lastPoint], smallVertices[3]);
        graph.addEdge(smallVertices[3], cornerVerticesMap[lastPoint]);
    }
}

void GregoryPatchCreator::reset() {
    holes.clear();
    holePoints.clear();
}

std::vector<std::vector<int>> GregoryPatchCreator::getHolePoints() {
    return holePoints;
}

void GregoryPatchCreator::prepareHole(int holeId) {
    patchSides.clear();
    std::vector<GregoryPatchGraphVertex> hole = holes[holeId];

    std::vector<std::vector<GregoryPatchGraphVertex>> sides;
    auto start = hole.begin();
    while(!start->isCornerVertex) {
        start++;
    }
    auto it = start;
    do {
        it++;
        sides.emplace_back();
        for(int i=0; i<4; i++) {
            if(it == hole.end()) it = hole.begin();
            sides.back().push_back(*(it++));
        }
    } while(it != hole.end() && it != start);

    for(auto &side : sides) {
        patchSides.emplace_back();
        patchSides.back()[0] = side[0];
        patchSides.back()[1] = side[1];
        patchSides.back()[2] = side[2];
        patchSides.back()[3] = side[3];
    }
}

void GregoryPatchCreator::setHole(std::vector<std::array<GregoryPatchGraphVertex, 4>> hole) {
    patchSides = std::move(hole);
}

void GregoryPatchCreator::fillHole(AppState &appState) {
    p1is.clear();
    p2is.clear();
    p3is.clear();
    fiSide.clear();
    fiMiddle.clear();
    fiLast.clear();

    std::vector<glm::vec3> Q;
    for(auto &side : patchSides) {
        assert(side.size() == 4);
        std::array<glm::vec3, 4> ctrlPoints{
                appState.pointSet[side[0].pointId]->position,
                appState.pointSet[side[1].pointId]->position,
                appState.pointSet[side[2].pointId]->position,
                appState.pointSet[side[3].pointId]->position
        };
        auto divided = divideDeCasteljau(ctrlPoints, 0.5);

        p3is.emplace_back();
        p3is.back()[0] = divided.first[0];
        p3is.back()[1] = divided.first[1];
        p3is.back()[2] = divided.first[2];
        p3is.back()[3] = divided.first[3]; // divided.second[0];
        p3is.back()[4] = divided.second[1];
        p3is.back()[5] = divided.second[2];
        p3is.back()[6] = divided.second[3];

        std::array<glm::vec3, 4> ctrlPoints1{
                appState.pointSet[side[0].outgoingBernstein[0]]->position,
                appState.pointSet[side[1].outgoingBernstein[0]]->position,
                appState.pointSet[side[2].outgoingBernstein[0]]->position,
                appState.pointSet[side[3].outgoingBernstein[0]]->position
        };
        auto divided1 = divideDeCasteljau(ctrlPoints1, 0.5);

        fiSide.emplace_back();
        fiSide.back()[0] = 2.f * divided.first[1] - divided1.first[1];
        fiSide.back()[1] = 2.f * divided.first[2] - divided1.first[2];
        fiSide.back()[2] = 2.f * divided.second[1] - divided1.second[1];
        fiSide.back()[3] = 2.f * divided.second[2] - divided1.second[2];

        auto p3i = divided.first[3]; // Middle point
        auto p4i = divided1.first[3]; // Middle point 1 on the back.
        auto p2i = p3i + (p3i - p4i); // C1
        auto qi = (3.f * p2i - p3i) / 2.f;
        Q.push_back(qi);

        p2is.push_back(p2i);
    }

    // Calculate middle point.
    p0 = glm::vec3(0);
    for(auto q : Q)
        p0 += q;
    p0 /= Q.size();

    for(int i = 0; i < patchSides.size(); i++) {
        auto q = Q[i];
        auto &side = patchSides[i];

        auto p1i = (2.f * q + p0) / 3.f;

        p1is.push_back(p1i);
    }

    for(int i = 0; i < patchSides.size(); i++) {
        int side = i;
        int sideNext = (i+1)%patchSides.size();
        int sidePrev = i==0? patchSides.size()-1 : i-1;

        glm::vec3 g0Right = p3is[side][4] - p3is[side][3];
        glm::vec3 g0Left = p3is[side][2] - p3is[side][3];

        glm::vec3 a = p0 - p1is[sidePrev];
        glm::vec3 b = p1is[sideNext] - p0;
        glm::vec3 g2Right = (a + b) / 2.f;
        glm::vec3 g2Left = -g2Right;

        glm::vec3 g1Right = (g0Right + g2Right) / 2.f;
        glm::vec3 g1Left = (g0Left + g2Left) / 2.f;

        glm::vec3 g01Right = deCasteljau2({g0Right, g1Right, g2Right}, 1/3.f);
        glm::vec3 g12Right = deCasteljau2({g0Right, g1Right, g2Right}, 2/3.f);
        glm::vec3 g01Left = deCasteljau2({g0Left, g1Left, g2Left}, 1/3.f);
        glm::vec3 g12Left = deCasteljau2({g0Left, g1Left, g2Left}, 2/3.f);

        fiMiddle.emplace_back();
        fiMiddle.back()[0] = p2is[side] + g01Left;
        fiMiddle.back()[1] = p2is[side] + g01Right;

        fiLast.emplace_back();
        fiLast.back()[0] = p1is[side] + g12Left;
        fiLast.back()[1] = p1is[side] + g12Right;

    }
}

std::pair<std::array<glm::vec3, 4>, std::array<glm::vec3, 4>>
GregoryPatchCreator::divideDeCasteljau(std::array<glm::vec3, 4> ctrlPoints, float t) {
    auto B00 = ctrlPoints[0];
    auto B01 = ctrlPoints[1];
    auto B02 = ctrlPoints[2];
    auto B03 = ctrlPoints[3];

    auto B10 = B00 * t + B01 * (1 - t);
    auto B11 = B01 * t + B02 * (1 - t);
    auto B12 = B02 * t + B03 * (1 - t);

    auto B20 = B10 * t + B11 * (1 - t);
    auto B21 = B11 * t + B12 * (1 - t);

    auto B30 = B20 * t + B21 * (1 - t);

    return {{B00, B10, B20, B30}, {B30, B21, B12, B03}};
}

glm::vec3 GregoryPatchCreator::deCasteljau2(std::array<glm::vec3, 3> ctrlPoints, float t) {
    auto B00 = ctrlPoints[0];
    auto B01 = ctrlPoints[1];
    auto B02 = ctrlPoints[2];

    auto B10 = B00 * t + B01 * (1 - t);
    auto B11 = B01 * t + B02 * (1 - t);

    auto B20 = B10 * t + B11 * (1 - t);

    return {B20};
}