//
// Created by faliszewskii on 29.05.24.
//

#include "GregoryPatchCreator.h"
#include "../math/graph/Graph.h"
#include "../math/graph/GregoryPatchGraphVertex.h"
#include "../state/AppState.h"
#include <CXXGraph/CXXGraph.hpp>

void  GregoryPatchCreator::findHoles(AppState &appState, const std::vector<int>& bezierPatchIds) {
    holePoints.clear();
    holes.clear();
    vId = 0;
    Graph<GregoryPatchGraphVertex> graph;
    std::map<int, int> cornerVerticesMap; // point id -> graph vertex id // Only for corner vertices.

    for(auto &id : bezierPatchIds) {
        auto &patch = *appState.patchC0Set[id];

        // TOP PATCH ROW
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
                        [&](int i, int j){return patch.wrapped? (i==patch.patchCountX-1 && j == 3 ? 0 : 4 * i + j) : 4 * i + j;},
                        [&](int i){return 4 * i + 0;},
                        [&](int i){return patch.wrapped? (i==patch.patchCountX-1 ? 0 : 4 * i + 3) : 4 * i + 3;},
                        [&](int i, int j){ return 4 * i + j + (4*patch.patchCountX); }
        );

        // BOTTOM PATCH ROW
        addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountX,
                        [&](int i, int j){return patch.wrapped? (i==0 && j == 0 ? patch.controlPoints.size() - 8 : patch.controlPoints.size() - 1) : patch.controlPoints.size() - 1 - (4 * i + j);},
                        [&](int i){return patch.wrapped? (i==0? patch.controlPoints.size() - 8 : patch.controlPoints.size() - 1 - (4 * i + 0)) : patch.controlPoints.size() - 1 - (4 * i + 0);},
                        [&](int i){return patch.controlPoints.size() - 1 - (4 * i + 3);},
                        [&](int i, int j){ return patch.controlPoints.size() - 1 - (4 * i + j) - (4*patch.patchCountX); }
        );

       if(!patch.wrapped) { // Only bottoms when a cylinder.
           int xSize = patch.patchCountX * 4;
           // LEFT PATCH COLUMN
           addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
                           [&](int i, int j){return xSize * (4 * i + j);},
                           [&](int i){return xSize * (4 * i + 0);},
                           [&](int i){return xSize * (4 * i + 3);},
                           [&](int i, int j){return xSize * (4 * i + j) + 1;}
           );

           // RIGHT PATCH COLUMN
           addPatchToGraph(appState, graph, cornerVerticesMap, patch, patch.patchCountY,
                           [&](int i, int j){return xSize * (4 * i + j) + (xSize-1);},
                           [&](int i){return xSize * (4 * i + 0) + (xSize-1);},
                           [&](int i){return xSize * (4 * i + 3) + (xSize-1);},
                           [&](int i, int j){return xSize * (4 * i + j) + (xSize-1) - 1;}
           );
       }

    }
    if(graph.vertices.empty()) {
        appState.logger.logDebug("Why empty?");
    }
    printCycle(appState, graph.dfs(), "DFS");

    // TODO Now cycles of length 12 + 3 corners = 15.
    holes = graph.findCycles(15);

    for(int i = 0; i < holes.size(); i++)
        printCycle(appState, holes[i], "hole " + std::to_string(i));

    std::transform(holes.begin(), holes.end(), std::back_inserter(holePoints), [](auto &el){
        std::vector<GregoryPatchGraphVertex> v1;
        std::copy_if(el.begin(), el.end(), std::back_inserter(v1), [](auto &inner) {return !inner.isCornerVertex;});
        std::vector<int> v;
        std::transform(v1.begin(), v1.end(), std::back_inserter(v), [&](auto &inner) { return inner.pointId; });

        std::unordered_set<int> s;
        auto last = std::remove_if(v.begin(), v.end(), [&](int n) {
            return (s.find(n) == s.end()) ? (s.insert(n), false) : true;
        });
        v.erase(last, v.end());

        return v;
    });

    std::stringstream ss;
    ss<<"Holes:";
    for(auto &i : holePoints) {
        for(auto &i1 :i)
            ss << i1 << ", ";
        ss << std::endl;
    }
    appState.logger.logDebug(ss.str());
}

void GregoryPatchCreator::addPatchToGraph(AppState &appState, Graph<GregoryPatchGraphVertex> &graph,
                                          std::map<int, int> &cornerVerticesMap,
                                          const PatchC0 &patch, int size, const std::function<int(int,int)>& small,
                                          const std::function<int(int)>& first, const std::function<int(int)>& last,
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
        int firstPoint = patch.controlPoints[first(i)].first; // Here
        int lastPoint = patch.controlPoints[last(i)].first; // Here
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

void GregoryPatchCreator::printCycle(AppState &state, std::vector<GregoryPatchGraphVertex> vector, std::string name) {
    std::stringstream ss;
    ss << name << ": ";
    for(auto &v : vector) {
        ss << v.pointId << "(" << (v.isCornerVertex? "C":"N") << ")" << ", ";
    }
    state.logger.logDebug(ss.str());
}

void GregoryPatchCreator::fillHole(AppState &appState, int holeId) {
    patchSides.clear();
    p1is.clear();
    p2is.clear();
    p3is.clear();
    fiSide.clear();
    fiMiddle.clear();
    fiLast.clear();
    std::vector<GregoryPatchGraphVertex> hole = holes[holeId];
    printCycle(appState, hole, "Filling hole");

    // Getting sides for deCastaljau

    std::vector<std::vector<GregoryPatchGraphVertex>> sides;
    auto start = hole.begin();
    while(!start->isCornerVertex) {
        appState.logger.logDebug("Skipping...");
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

    for(int i=0 ; i<sides.size(); i++) {
        std::stringstream ss;
        ss << "side "<< i <<": ";
        for(auto &el : sides[i])
            ss << el.pointId << "(" << el.isCornerVertex << "), ";
        appState.logger.logDebug(ss.str());
    }

    // Here we have the sides. Time to do deCasteljau

    for(auto &side : sides) {
        patchSides.emplace_back();
        patchSides.back()[0] = side[0];
        patchSides.back()[1] = side[1];
        patchSides.back()[2] = side[2];
        patchSides.back()[3] = side[3];
    }

    std::stringstream ss;
    ss << "Generating patch points:\n";
    int counter = 0;

    std::vector<glm::vec3> Q;
    for(auto &side : sides) {
        ss << "Side No." << counter++ << "\n";
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

        ss << "P3s:\n";/*
        appState.eventPublisher.publish(CreatePointEvent{divided.first[0]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.first[1]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.first[2]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.first[3]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.second[0]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.second[1]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.second[2]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << ", ";
        appState.eventPublisher.publish(CreatePointEvent{divided.second[3]});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0,0.5,1);
        ss << appState.lastIdCreated << "\n";*/


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

        fiMiddle.emplace_back();
        fiMiddle.back()[0] = fiSide.back()[1];
        fiMiddle.back()[1] = fiSide.back()[2];

        auto p3i = divided.first[3]; // Middle point
        auto p4i = divided1.first[3]; // Middle point 1 on the back.
        auto p2i = p3i + (p3i - p4i); // C1
        auto qi = (3.f * p2i - p3i) / 2.f;
        Q.push_back(qi);

        p2is.push_back(p2i);

        /*appState.eventPublisher.publish(CreatePointEvent{p2i});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(0,0.5,0.5,1);
        ss << "P2: " << appState.lastIdCreated << "\n";*/
    }

    // Calculate middle point.
    p0 = glm::vec3(0);
    for(auto q : Q)
        p0 += q;
    p0 /= 3.f;

    /*appState.eventPublisher.publish(CreatePointEvent{p0});
    appState.pointSet[appState.lastIdCreated]->color = glm::vec4(1,0.5,0.0,1);
    ss << "Global P0: " << appState.lastIdCreated << "\n";*/

    for(int i = 0; i < sides.size(); i++) {
        auto q = Q[i];
        auto &side = sides[i];

        auto p1i = (2.f * q + p0) / 3.f;

        p1is.push_back(p1i);

        /*appState.eventPublisher.publish(CreatePointEvent{p1i});
        appState.pointSet[appState.lastIdCreated]->color = glm::vec4(0.5,0,1,1);
        ss << "P1i:" << appState.lastIdCreated << "\n";*/
    }

    for(int i = 0; i < sides.size(); i++) {
        int side = i;
        int sideNext = (i+1)%patchSides.size();
        int sidePrev = i==0? patchSides.size()-1 : i-1;

        fiLast.emplace_back();
        fiLast.back()[0] = p1is[side] + (p1is[sidePrev] - p0);
        fiLast.back()[1] = p1is[side] + (p1is[sideNext] - p0);
    }
//    appState.logger.logDebug(ss.str());
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
