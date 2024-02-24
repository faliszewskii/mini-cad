//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_MESH_H
#define OPENGL_SANDBOX_MESH_H


#include "../SceneNode.h"
#include "../shader/Shader.h"
#include "../material/Material.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh : public SceneNode {
    std::vector<Vertex> vertices;
    std::optional<std::vector<unsigned int>> indices;
public:
    int drawingMode;
    std::optional<std::reference_wrapper<Material>> material;

    explicit Mesh(std::string name, std::vector<Vertex> vertices = std::vector<Vertex>(), std::optional<std::vector<unsigned int>> indices = {},
         std::optional<std::reference_wrapper<Material>> = {}, int drawingMode = GL_TRIANGLES);

    std::string getTypeName() override { return "Mesh"; };

    void render();

    void update(std::vector<Vertex> &&vertices, std::optional<std::vector<unsigned int>> &&indices);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


#endif //OPENGL_SANDBOX_MESH_H
