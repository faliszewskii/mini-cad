//
// Created by faliszewskii on 07.01.24.
//
#include <utility>

#include "Mesh.h"

Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::optional<std::vector<unsigned int>> indices,
           std::optional<std::reference_wrapper<Material>> material, int drawingMode) : SceneNode(std::move(name)),
                              vertices(std::move(vertices)), indices(std::move(indices)),
                              material(material), drawingMode(drawingMode) {
    setupMesh();
}

void Mesh::render() {
    // draw mesh
    glBindVertexArray(VAO);
    if (indices)
        glDrawElements(drawingMode, indices.value().size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(drawingMode, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (indices) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * sizeof(unsigned int), &indices.value()[0],
                     GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::update(std::vector<Vertex> &&newVertices, std::optional<std::vector<unsigned int>> &&newIndices) {
    vertices = std::move(newVertices);
    indices = std::move(newIndices);
    setupMesh();
}
