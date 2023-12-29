//
// Created by faliszewskii on 28.12.23.
//

#ifndef OPENGL_SANDBOX_MODELNODE_H
#define OPENGL_SANDBOX_MODELNODE_H


#include <glm/gtx/quaternion.hpp>
#include "../shader/shader.h"
#include "Mesh.h"

class ModelNode {
    std::vector<Mesh> meshes;
    std::vector<ModelNode> children;
    glm::mat4 transformation;
//    glm::vec3 translation;
//    glm::quat rotation;
//    glm::vec3 scale;

public:
    ModelNode(std::vector<Mesh> meshes, std::vector<ModelNode> children, glm::mat4 mat);
    void draw(Shader &shader, glm::mat4 model);
};


#endif //OPENGL_SANDBOX_MODELNODE_H
