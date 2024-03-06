//
// Created by faliszewskii on 07.01.24.
//
#include "Transformation.h"
#include "../../algebra/AlgebraUtils.h"

#include <utility>


Transformation::Transformation() : translation(),
orientation(glm::qua<double>(1, 0, 0, 0)), scale(glm::vec<3, double>(1.0f)) {}

Transformation::Transformation(glm::vec<3, double> translation, glm::qua<double> orientation, glm::vec<3, double> scale) :
        translation(translation), orientation(orientation), scale(scale)  {}

Transformation::Transformation(glm::mat<4, 4, double> transformation) {
    glm::vec<3, double> scaleFactor{};
    glm::qua<double> orientationFactor{};
    glm::vec<3, double> translationFactor{};
    AlgebraUtils::decomposeMtx(transformation, translationFactor, orientationFactor, scaleFactor);
    setTranslation(translationFactor);
    setOrientation(orientationFactor);
    setScale(scaleFactor);
}

glm::mat<4,4,double> Transformation::getTransformation() const {
    return glm::translate(glm::mat<4,4,double>(1.0f), translation) * glm::mat4_cast(orientation) * glm::scale(glm::mat<4,4,double>(1.0f), scale);
}

void Transformation::setTransformation(glm::mat<4,4,double> transformation) {
    glm::vec<3, double> scale{};
    glm::qua<double> rotation{};
    glm::vec<3, double> translation{};
    AlgebraUtils::decomposeMtx(transformation, translation, rotation, scale);
    setTranslation(translation);
    setOrientation(glm::normalize(rotation));
    setScale(scale);
}

void Transformation::setTranslation(glm::vec<3, double> newTranslation) {
    translation = newTranslation;
}

void Transformation::setOrientation(glm::qua<double> newOrientation) {
    orientation = newOrientation;
}

void Transformation::addRotation(glm::vec<3, double> eulerAngles) {
    auto x = glm::angleAxis(eulerAngles.x, glm::vec<3, double>(1,0,0));
    auto y = glm::angleAxis(eulerAngles.y, glm::vec<3, double>(0,1,0));
    auto z = glm::angleAxis(eulerAngles.z, glm::vec<3, double>(0,0,1));
    orientation *= glm::normalize(z * y * x);
}

void Transformation::setScale(glm::vec<3, double> newScale) {
    scale = newScale;
}

glm::vec<3, double> Transformation::getRotationAngles() const {
    return glm::eulerAngles(orientation);
}

void Transformation::setRotation(glm::vec<3, double> rotation) {
    orientation = glm::quat(rotation);
}
