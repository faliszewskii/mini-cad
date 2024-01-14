//
// Created by faliszewskii on 10.01.24.
//

#include <glm/gtc/quaternion.hpp>
#include <utility>
#include "TransformationProperty.h"

TransformationProperty::TransformationProperty(): position(), orientation(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1.0f)) {}

TransformationProperty::TransformationProperty(Bindable<glm::vec3> position, Bindable<glm::quat> orientation, Bindable<glm::vec3> scale) :
position(std::move(position)), orientation(std::move(orientation)), scale(std::move(scale)) {}

glm::mat4 TransformationProperty::getTransformation() {
    return glm::translate(glm::mat4(1.0f), position.get()) * glm::scale(glm::mat4(1.0f), scale.get()) * glm::mat4_cast(orientation.get());
}

void TransformationProperty::setPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void TransformationProperty::setOrientation(glm::quat newOrientation) {
    orientation = newOrientation;
}

void TransformationProperty::setOrientation(glm::vec3 eulerAngles) {
    // TODO
}

void TransformationProperty::setScale(glm::vec3 newScale) {
    scale = newScale;
}

