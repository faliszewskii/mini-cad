//
// Created by faliszewskii on 07.01.24.
//
#include "Transformation.h"
#include "../../algebra/AlgebraUtils.h"

#include <utility>


Transformation::Transformation() : translation(),
orientation(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1.0f)) {}

Transformation::Transformation(glm::vec3 translation, glm::quat orientation, glm::vec3 scale) :
        translation(translation), orientation(orientation), scale(scale)  {}

Transformation::Transformation(glm::mat4 transformation) {
    glm::vec3 scaleFactor{};
    glm::quat orientationFactor{};
    glm::vec3 translationFactor{};
    AlgebraUtils::decomposeMtx(transformation, translationFactor, orientationFactor, scaleFactor);
    setTranslation(translationFactor);
    setOrientation(orientationFactor);
    setScale(scaleFactor);
}

glm::mat4 Transformation::getTransformation() const {
    return glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1.0f), scale);
}

void Transformation::setTransformation(glm::mat4 transformation) {
    glm::vec3 scale{};
    glm::quat rotation{};
    glm::vec3 translation{};
    AlgebraUtils::decomposeMtx(transformation, translation, rotation, scale);
    setTranslation(translation);
    setOrientation(glm::normalize(rotation));
    setScale(scale);
}

void Transformation::setTranslation(glm::vec3 newTranslation) {
    translation = newTranslation;
}

void Transformation::setOrientation(glm::quat newOrientation) {
    orientation = newOrientation;
}

void Transformation::addRotation(glm::vec3 eulerAngles) {
    auto z = glm::angleAxis(eulerAngles.z, glm::vec3(0,0,1));
    auto y = glm::angleAxis(eulerAngles.y, glm::vec3(0,1,0));
    auto x = glm::angleAxis(eulerAngles.x, glm::vec3(1,0,0));
    orientation *= glm::normalize(x * y * z);
}

void Transformation::setScale(glm::vec3 newScale) {
    scale = newScale;
}

glm::vec3 Transformation::getRotationAngles() const {
    return glm::eulerAngles(orientation);
}

void Transformation::setRotation(glm::vec3 rotation) {
    orientation = glm::quat(rotation);
}
