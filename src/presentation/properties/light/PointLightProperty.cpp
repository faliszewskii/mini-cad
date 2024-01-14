//
// Created by faliszewskii on 12.01.24.
//

#include "PointLightProperty.h"

#include <utility>

std::string PointLightProperty::getPropertyName() {
    return "Point Light";
}

void PointLightProperty::acceptVisit(PropertyVisitor &visitor) {
    visitor.visitPointLightProperty(*this);
}

PointLightProperty::PointLightProperty(Bindable<glm::vec3> position) : position(std::move(position)) {}

