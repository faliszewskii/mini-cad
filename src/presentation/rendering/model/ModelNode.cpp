//
// Created by faliszewskii on 28.12.23.
//

#define UUID_SYSTEM_GENERATOR
#include "../../lib/uuid/uuid.h"
#include "ModelNode.h"

#include <utility>

ModelNode::ModelNode(std::string name, std::vector<MeshOld> meshes, std::vector<ModelNode> children,
                     glm::mat4 transformation): name(std::move(name)), meshes(std::move(meshes)), children(std::move(children)),
                     transformation(transformation), uniqueObjectId(uuids::uuid_system_generator{}()) {}

ModelNode::ModelNode(std::string name) : name(std::move(name)), transformation(glm::mat4(1.0f)),
                    uniqueObjectId(uuids::uuid_system_generator{}())  {}

void ModelNode::render(Shader &shader, glm::mat4 model) {
    model = model * transformation;
    shader.setMat4("model", model);
    for(auto & mesh : meshes)
        mesh.render(shader);
    for(auto & child : children)
        child.render(shader, model);
}
