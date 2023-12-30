//
// Created by faliszewskii on 28.12.23.
//

#define UUID_SYSTEM_GENERATOR
#include "../../lib/uuid/uuid.h"
#include "ModelNode.h"

ModelNode::ModelNode(std::string name, std::vector<Mesh> meshes, std::vector<ModelNode> children,
                     glm::mat4 transformation): name(name), meshes(std::move(meshes)), children(std::move(children)),
                     transformation(transformation), uniqueObjectId(uuids::uuid_system_generator{}()) {}

ModelNode::ModelNode(std::string name) : name(name), transformation(glm::mat4(1.0f)),
                    uniqueObjectId(uuids::uuid_system_generator{}())  {}

void ModelNode::render(Shader &shader, glm::mat4 model) {
    model = model * transformation;
    shader.setMat4("model", model);
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].render(shader);
    for(unsigned int i = 0; i < children.size(); i++)
        children[i].render(shader, model);
}
