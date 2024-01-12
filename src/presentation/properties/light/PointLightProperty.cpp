//
// Created by faliszewskii on 12.01.24.
//

#include "PointLightProperty.h"

std::string PointLightProperty::getPropertyName() {
    return "Point Light";
}

void PointLightProperty::acceptVisit(PropertyVisitor &visitor) {
    visitor.visitPointLightProperty(*this);
}

PointLightProperty::PointLightProperty() {}

PointLightProperty::PointLightProperty(glm::vec3 position) : position(position) {}

