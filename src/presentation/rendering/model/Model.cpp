//
// Created by faliszewskii on 28.12.23.
//

#include "Model.h"

#include <utility>

Model::Model(ModelNode rootNode) : rootNode(std::move(rootNode)), translation(0), rotation(1, 0, 0, 0), scale(0.1) {}

void Model::draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        model = glm::scale(model, scale);
//        model = glm::ro
    rootNode.draw(shader, model);
}

Model::~Model() {
    // TODO
}
