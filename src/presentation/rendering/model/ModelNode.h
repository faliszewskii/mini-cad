//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_MODELNODE_H
#define OPENGL_SANDBOX_MODELNODE_H


#include <glm/gtx/quaternion.hpp>
#include "../shader/shader.h"
#include "Mesh.h"

class ModelNode {
    uuids::uuid uniqueObjectId;
    std::string name;
    std::vector<Mesh> meshes;
    std::vector<ModelNode> children;
    glm::mat4 transformation;
//    glm::vec3 translation;
//    glm::quat rotation;
//    glm::vec3 scale;

public:
    ModelNode(std::string name);
    ModelNode(std::string name, std::vector<Mesh> meshes, std::vector<ModelNode> children, glm::mat4 mat);
    void render(Shader &shader, glm::mat4 model);
    std::vector<ModelNode>& getChildren() { return children; };
    std::vector<Mesh>& getMeshes() { return meshes; };
    std::string getName() { return name; };
    uuids::uuid getUuid() { return uniqueObjectId; };
};


#endif //OPENGL_SANDBOX_MODELNODE_H
