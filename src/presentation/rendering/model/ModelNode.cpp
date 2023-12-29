//
// Created by faliszewskii on 28.12.23.
//

#include "ModelNode.h"

void ModelNode::draw(Shader &shader, glm::mat4 model) {
    model = model * transformation;
    shader.setMat4("model", model);
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
    for(unsigned int i = 0; i < children.size(); i++)
        children[i].draw(shader, model);
}

ModelNode::ModelNode(std::vector<Mesh> meshes, std::vector<ModelNode> children, glm::mat4 transformation)
        : meshes(std::move(meshes)), children(std::move(children)), transformation(transformation) {}
