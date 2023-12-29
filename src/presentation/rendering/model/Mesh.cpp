//
// Created by faliszewskii on 28.12.23.
//

#include "Mesh.h"

#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::optional<std::vector<unsigned int>> indices,
           std::optional<std::vector<Texture>> textures, int drawingMode) :
           vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)),
           drawingMode(drawingMode)
{
    setupMesh();
}

void Mesh::draw(Shader &shader) {
    if(textures) {
        for(unsigned int i = 0; i < textures.value().size(); i++)
        {
            // TODO Possible handling of multiple textures of the same type.
            glActiveTexture(GL_TEXTURE0 + i);
            shader.setInt(/*"material." + */textures.value()[i].type, i);
            glBindTexture(GL_TEXTURE_2D, textures.value()[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    // draw mesh
    glBindVertexArray(VAO);
    if(indices)
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

    if(indices) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * sizeof(unsigned int), &indices.value()[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
