//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GRAPH_H
#define OPENGL_SANDBOX_GRAPH_H

#include <vector>
#include <optional>
#include <cassert>
#include <unordered_set>

template<typename TVertex>
struct Graph {
    std::vector<TVertex> vertices;
    std::vector<std::vector<unsigned int>> neighbourList;

    unsigned int addVertex(TVertex vertex);
    void addEdge(unsigned int v1, int unsigned v2);

    std::vector<TVertex> dfs();

    std::vector<std::vector<TVertex>> findCycles(int length);
    std::vector<std::vector<TVertex>> findCycles(int start, int length);

private:

    std::vector<TVertex> dfs(std::vector<bool> &visited, int v);

    std::vector<std::vector<TVertex>> findCycles(int start, int v, int length, std::vector<bool> visited);

    bool cyclesEqual(std::vector<TVertex> &c1, std::vector<TVertex> &c2) {
        for(auto &el : c1) {
            bool exists = false;
            for(auto &o : c2) {
                exists |= el == o;
            }
            if(!exists) return false;
        }
        return true;
    }
};

template<typename TVertex>
std::vector<std::vector<TVertex>> Graph<TVertex>::findCycles(int start, int v, int length, std::vector<bool> visited) {
    visited[v] = true;
    if(length <= 1) {
        for(auto &neighbour : neighbourList[v]) {
            if (neighbour == start)
                return {{vertices[v]}};
        }
        return {};
    }
    std::vector<std::vector<TVertex>> cycles;
    for(auto &neighbour : neighbourList[v]) {
        if(visited[neighbour] == true) continue;
        auto results = findCycles(start, neighbour, length - 1, visited);
        for(auto &result : results) {
            result.push_back(vertices[v]);
            cycles.push_back(result);
        }
    }
    return cycles;
}

template<typename TVertex>
std::vector<std::vector<TVertex>> Graph<TVertex>::findCycles(int start, int length) {
    std::vector<bool> visited(vertices.size(), false);
    return findCycles(start, start, length, visited);
}

template<typename TVertex>
std::vector<std::vector<TVertex>> Graph<TVertex>::findCycles(int length) {
    std::vector<std::vector<TVertex>> cycles;

    for(int i = 0; i < vertices.size(); i++) {
        auto cycleFromI = findCycles(i, length);
        cycles.insert(cycles.end(), cycleFromI.begin(), cycleFromI.end());
    }

    std::vector<std::vector<TVertex>> uniqueCycles;
    for(auto &cycle : cycles) {
        if(std::any_of(uniqueCycles.begin(), uniqueCycles.end(), [&](auto &o){return cyclesEqual(cycle, o);}))
            continue;
        uniqueCycles.push_back(cycle);
    }

    return uniqueCycles;
}

template<typename TVertex>
std::vector<TVertex> Graph<TVertex>::dfs() {
    std::vector<bool> visited(vertices.size());
    std::vector<TVertex> result;
    for(int i = 0; i < visited.size(); i++) {
        if(!visited[i]) {
            auto v = dfs(visited, i);
            result.insert(result.end(), v.begin(), v.end());
        }
    }
    return result;
}

template<typename TVertex>
std::vector<TVertex> Graph<TVertex>::dfs(std::vector<bool> &visited, int v) {
    std::vector<TVertex> list;

    list.push_back(vertices[v]);
    visited[v] = true;

    for(int i = 0; i < neighbourList[v].size(); i++) {
        int neighbour = neighbourList[v][i];
        if(!visited[neighbour]) {
            auto childList = dfs(visited, neighbour);
            list.insert(list.end(), childList.begin(), childList.end());
        }
    }
    return list;
}



template<typename TVertex>
void Graph<TVertex>::addEdge(unsigned int v1, unsigned int v2) {
    neighbourList[v1].push_back(v2);
}

template<typename TVertex>
unsigned int Graph<TVertex>::addVertex(TVertex vertex) {
    vertices.push_back(vertex);
    neighbourList.emplace_back();
    return vertices.size() - 1;
}

#endif //OPENGL_SANDBOX_GRAPH_H
