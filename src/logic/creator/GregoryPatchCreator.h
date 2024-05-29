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
    std::vector<std::vector<unsigned int>> holes;
public:
    void reset();
    std::vector<std::vector<unsigned int>> getHoles();

    void findHoles(AppState &appState, const std::vector<unsigned int>& bezierPatchIds);

    void addPatchToGraph(AppState &appState, Graph<GregoryPatchGraphVertex> &graph,
                    std::map<int, int> &cornerVerticesMap,
                    const PatchC0 &patch, int size, const std::function<int(int,int)>& small,
                    const std::function<int(int)>& first, const std::function<int(int)> &last);
};


#endif //OPENGL_SANDBOX_GREGORYPATCHCREATOR_H
