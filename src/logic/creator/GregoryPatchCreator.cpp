//
// Created by faliszewskii on 29.05.24.
//

#include "GregoryPatchCreator.h"
#include "../math/graph/Graph.h"
#include "../math/graph/GregoryPatchGraphVertex.h"
#include "../state/AppState.h"
#include <CXXGraph/CXXGraph.hpp>

void  GregoryPatchCreator::findHoles(AppState &appState, const std::vector<unsigned int>& bezierPatchIds) {
    Graph<GregoryPatchGraphVertex> graph;

    GregoryPatchGraphVertex test{0, 0, false, {}};

    std::vector<std::vector<unsigned int>> cycles;

    std::map<int, int> cornerVerticesMap; // point id -> graph vertex id // Only for corner vertices.
    // Tworzymy 4 słabe wierzchołki i łączymy je skrajnie do dwóch super wierzchołków. I tak dla każdej krawędzi płatków.
    // Jeżeli nie istnieje w mapie taki super wierzchołek to go tworzymy i dodajemy do mapy.
    for(auto &id : bezierPatchIds) {
        auto &patch = *appState.patchC0Set[id];

        // TOP PATCH ROW
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
                        [&](int i, int j){return 4 * i + j;},
                        [&](int i){return 4 * i + 0;},
                        [&](int i){return 4 * i + 3;}
        );

        // BOTTOM PATCH ROW
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
                        [&](int i, int j){return patch.controlPoints.size() - 1 - (4 * i + j);},
                        [&](int i){return patch.controlPoints.size() - 1 - (4 * i + 0);},
                        [&](int i){return patch.controlPoints.size() - 1 - (4 * i + 3);}
        );

        int xSize = patch.patchCountX * 4;
        // LEFT PATCH COLUMN
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
                        [&](int i, int j){return xSize * (4 * i + j);},
                        [&](int i){return xSize * (4 * i + 0);},
                        [&](int i){return xSize * (4 * i + 3);}
        );

        // RIGHT PATCH COLUMN
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
                        [&](int i, int j){return xSize * (4 * i + j) + (xSize-1);},
                        [&](int i){return xSize * (4 * i + 0) + (xSize-1);},
                        [&](int i){return xSize * (4 * i + 3) + (xSize-1);}
        );

    }
    std::stringstream ss;
    ss << "DFS: ";
    for(auto &v : graph.dfs()) {
        ss << v.pointId << "(" << (v.isCornerVertex? "C":"N") << ")" << ", ";
    }
    appState.logger.logDebug(ss.str());
    holes = cycles;
}

void GregoryPatchCreator::addPatchToGraph(AppState &appState, Graph<GregoryPatchGraphVertex> &graph,
                                          std::map<int, int> &cornerVerticesMap,
                                          const PatchC0 &patch, int size, const std::function<int(int,int)>& small,
                                          const std::function<int(int)>& first, const std::function<int(int)>& last) {
    for(int i = 0; i < size; i++) {
        std::array<int, 4> smallVertices{};
        for(int j = 0; j < 4; j++) {
            int smallVertex = patch.controlPoints[small(i, j)].first; // Here
            smallVertices[j] = graph.addVertex({smallVertex, patch.id, false});
        }
        for(int j = 0; j < 3; j++) {
            graph.addEdge(smallVertices[j], smallVertices[j + 1]);
            graph.addEdge(smallVertices[j + 1], smallVertices[j]);
        }
        int firstPoint = patch.controlPoints[first(i)].first; // Here
        int lastPoint = patch.controlPoints[last(i)].first; // Here
        if(!cornerVerticesMap.contains(firstPoint)) {
            int corner = graph.addVertex({firstPoint, patch.id, true});
            cornerVerticesMap.insert({firstPoint, corner});
        }
        if(!cornerVerticesMap.contains(lastPoint)) {
            int corner = graph.addVertex({lastPoint, patch.id, true});
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
}

std::vector<std::vector<unsigned int>> GregoryPatchCreator::getHoles() {
    return holes;
}
