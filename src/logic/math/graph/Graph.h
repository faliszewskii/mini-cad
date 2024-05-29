//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GRAPH_H
#define OPENGL_SANDBOX_GRAPH_H

#include <vector>

template<typename TVertex>
struct Graph {
    std::vector<TVertex> vertices;
    std::vector<std::vector<unsigned int>> neighbourList;

    unsigned int addVertex(TVertex vertex);
    void addEdge(unsigned int v1, int unsigned v2);

    std::vector<std::vector<TVertex>> getCycles();

    std::vector<TVertex> dfs();
private:

    std::vector<TVertex> dfs(std::vector<bool> &visited, int v);
};

template<typename TVertex>
std::vector<TVertex> Graph<TVertex>::dfs() {
    std::vector<bool> visited(vertices.size());
    return dfs(visited, 0);
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

template<typename TVertex>
std::vector<std::vector<TVertex>> Graph<TVertex>::getCycles() {
    return std::vector<std::vector<TVertex>>();
}

#endif //OPENGL_SANDBOX_GRAPH_H
