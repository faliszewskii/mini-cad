//
// Created by faliszewskii on 07.01.24.
//

#include "Light.h"

#include <utility>

Light::Light(std::string name, glm::vec3 position) : SceneNode(std::move(name)), position(position) {}

int Light::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitLight(*this);
}

int Light::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveLight(*this);
}
