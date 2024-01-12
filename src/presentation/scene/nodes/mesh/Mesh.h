//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_MESH_H
#define OPENGL_SANDBOX_MESH_H


#include "../../tree/SceneNode.h"
#include "../shader/Shader.h"
#include "../material/Material.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
//    glm::vec3 tangent; TODO
//    glm::vec3 bitangent;
//    int m_BoneIDs[MAX_BONE_INFLUENCE];
//    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh : public SceneNode {
public:
    std::vector<Vertex>                         vertices;
    std::optional<std::vector<unsigned int>>    indices;
    int drawingMode;

    Mesh(std::string name, std::vector<Vertex> vertices, std::optional<std::vector<unsigned int>> indices = {}, int drawingMode = GL_TRIANGLES);
    std::string getTypeName() override { return "Mesh"; };

    void render(Shader &shader, glm::mat4 mat);

    int acceptVisit(SceneNodeVisitor& visitor) override;
    int acceptLeave(SceneNodeVisitor& visitor) override;
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


#endif //OPENGL_SANDBOX_MESH_H
