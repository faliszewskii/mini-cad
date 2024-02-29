//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_MESH_H
#define OPENGL_SANDBOX_MESH_H


#include "../SceneNode.h"
#include "../shader/Shader.h"
#include "../material/Material.h"
#include "../../vertices/is_vertex.h"

template<typename TVertex> requires is_vertex<TVertex>
class Mesh : public SceneNode {
    std::vector<TVertex> vertices;
    std::optional<std::vector<unsigned int>> indices;
public:
    int drawingMode;
    std::optional<std::reference_wrapper<Material>> material{};

    explicit Mesh(std::string name, std::vector<TVertex> vertices = std::vector<TVertex>(), std::optional<std::vector<unsigned int>> indices = {},
         std::optional<std::reference_wrapper<Material>> material = {}, int drawingMode = GL_TRIANGLES) : SceneNode(std::move(name)),
    vertices(std::move(vertices)), indices(std::move(indices)),
    material(material), drawingMode(drawingMode) {
        setupMesh();
    }

    std::string getTypeName() override { return "Mesh"; };

    void render() const {
        // draw mesh
        glBindVertexArray(VAO);
        if (indices)
            glDrawElements(drawingMode, indices.value().size(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(drawingMode, 0, vertices.size());
        glBindVertexArray(0);
    }

    void update(std::vector<TVertex> &&newVertices, std::optional<std::vector<unsigned int>> &&newIndices) {
        vertices = std::move(newVertices);
        indices = std::move(newIndices);
        setupMesh();
    }

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], GL_STATIC_DRAW);

        if (indices) {
            glGenBuffers(1, &EBO);
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
