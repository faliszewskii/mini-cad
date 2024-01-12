//
// Created by faliszewskii on 07.01.24.
//

#include "Light.h"

#include <utility>
#include "../../../properties/light/PointLightProperty.h"

Light::Light(std::string name, glm::vec3 position) : SceneNode(std::move(name)), pointLightProperty(position),
                                                     currentLightType(std::ref(pointLightProperty.value())){}

int Light::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitLight(*this);
}

int Light::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveLight(*this);
}

std::vector<std::reference_wrapper<Property>> Light::getProperties() {
    std::vector<std::reference_wrapper<Property>> vec{currentLightType};
    return vec;
}
