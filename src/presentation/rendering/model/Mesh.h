//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_MESH_H
#define OPENGL_SANDBOX_MESH_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <optional>
#include "../shader/shader.h"

//#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
//    glm::vec3 tangent; TODO
//    glm::vec3 bitangent;
//    int m_BoneIDs[MAX_BONE_INFLUENCE];
//    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>                         vertices;
    std::optional<std::vector<unsigned int>>    indices;
    std::optional<std::vector<Texture>>         textures;
    int drawingMode;

    Mesh(std::vector<Vertex> vertices, std::optional<std::vector<unsigned int>> indices = {},
     std::optional<std::vector<Texture>> textures = {}, int drawingMode = GL_TRIANGLES);

    void draw(Shader &shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


#endif //OPENGL_SANDBOX_MESH_H
