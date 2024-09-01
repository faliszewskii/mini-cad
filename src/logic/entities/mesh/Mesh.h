//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_MESH_H
#define OPENGL_SANDBOX_MESH_H


#include "../shader/Shader.h"
#include "../../vertices/is_vertex.h"

template<typename TVertex> requires is_vertex<TVertex>
class Mesh {
    std::vector<TVertex> vertices;
    std::optional<std::vector<unsigned int>> indices;
public:
    int drawingMode;

    explicit Mesh(std::vector<TVertex> vertices = std::vector<TVertex>(), std::optional<std::vector<unsigned int>> indices = {},
         int drawingMode = GL_TRIANGLES) : vertices(std::move(vertices)), indices(std::move(indices)), drawingMode(drawingMode) {
        setupMesh();
    }

    void render(int instanceCount = 1) const {
        // draw mesh
        glBindVertexArray(VAO);
        if (indices)
            glDrawElementsInstanced(drawingMode, indices.value().size(), GL_UNSIGNED_INT, 0, instanceCount);
        else
            glDrawArraysInstanced(drawingMode, 0, vertices.size(), instanceCount);
        glBindVertexArray(0);
    }

    void update(std::vector<TVertex> &&newVertices, std::optional<std::vector<unsigned int>> &&newIndices) {
        vertices = std::move(newVertices);
        indices = std::move(newIndices);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], GL_STATIC_DRAW);

        if (indices) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * sizeof(unsigned int), &indices.value()[0],
                         GL_STATIC_DRAW);
        }

        glBindVertexArray(0);
    }

    void update(TVertex vertex, int i) {
        vertices[i] = vertex;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(TVertex), sizeof(TVertex), &vertex);

        glBindVertexArray(0);
    }

    std::vector<TVertex> getVertices() { return vertices; };
    std::optional<std::vector<unsigned int>> getIndices() const { return indices; };

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], GL_STATIC_DRAW);

        if (indices) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * sizeof(unsigned int), &indices.value()[0],
                         GL_STATIC_DRAW);
        }

        for(int i = 0; i < TVertex::getSizes().size(); i++) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, TVertex::getSizes()[i], TVertex::getTypes()[i], GL_FALSE,
                                  sizeof(TVertex), reinterpret_cast<void*>(TVertex::getOffsets()[i]));
        }
        glBindVertexArray(0);
    }
};


#endif //OPENGL_SANDBOX_MESH_H
