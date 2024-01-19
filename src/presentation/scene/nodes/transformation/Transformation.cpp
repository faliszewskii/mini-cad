//
// Created by faliszewskii on 07.01.24.
//
#include "Transformation.h"
#include "../../../../logic/algebra/AlgebraUtils.h"

#include <utility>


Transformation::Transformation(std::string name) : SceneNode(std::move(name)), translation(),
orientation(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1.0f)) {}

Transformation::Transformation(std::string name, Bindable<glm::vec3> translation, Bindable<glm::quat> orientation, Bindable<glm::vec3> scale) :
        SceneNode(std::move(name)), translation(std::move(translation)), orientation(std::move(orientation)), scale(std::move(scale))  {}

Transformation::Transformation(std::string name, glm::mat4 transformation) : SceneNode(std::move(name)) {
    glm::vec3 scaleFactor;
    glm::quat orientationFactor;
    glm::vec3 translationFactor;
    AlgebraUtils::decomposeMtx(transformation, translationFactor, orientationFactor, scaleFactor);
    setTranslation(translationFactor);
    setOrientation(orientationFactor);
    setScale(scaleFactor);
}

glm::mat4 Transformation::getTransformation() {
    return glm::translate(glm::mat4(1.0f), translation.get()) * glm::mat4_cast(orientation.get()) *
           glm::scale(glm::mat4(1.0f), scale.get());
}

void Transformation::setTransformation(glm::mat4 transformation) {
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    AlgebraUtils::decomposeMtx(transformation, translation, rotation, scale);
    setTranslation(translation);
    setOrientation(rotation);
    setScale(scale);
}

void Transformation::setTranslation(glm::vec3 newTranslation) {
    translation = newTranslation;
}

void Transformation::setOrientation(glm::quat newOrientation) {
    orientation = newOrientation;
}

void Transformation::setOrientation(glm::vec3 eulerAngles) {
    throw "not implemented";
}

void Transformation::setScale(glm::vec3 newScale) {
    scale = newScale;
}

int Transformation::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitTransformation(*this);
}

int Transformation::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveTransformation(*this);
}