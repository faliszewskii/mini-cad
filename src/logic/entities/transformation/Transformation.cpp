//
// Created by faliszewskii on 07.01.24.
//
#include "Transformation.h"
#include "../../math/algebra/AlgebraUtils.h"

#include <utility>


Transformation::Transformation() : translation(),
orientation(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1.0f)) {}

Transformation::Transformation(glm::vec3 translation, glm::quat orientation, glm::vec3 scale) :
        translation(translation), orientation(orientation), scale(scale)  {}

glm::mat4 Transformation::getTransformation() const {
    auto eulerRotation = glm::eulerAngles(orientation);
    glm::mat4 T = {
            {1,0,0,0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {translation.x, translation.y, translation.z, 1}
    };
    glm::mat4 Rx = {
            {1,0,0,0},
            {0, cos(eulerRotation.x), sin(eulerRotation.x), 0},
            {0, -sin(eulerRotation.x), cos(eulerRotation.x), 0},
            {0, 0, 0, 1}
    };
    glm::mat4 Ry = {
            {cos(eulerRotation.y),0,-sin(eulerRotation.y),0},
            {0, 1, 0, 0},
            {sin(eulerRotation.y), 0, cos(eulerRotation.y), 0},
            {0, 0, 0, 1}
    };
    glm::mat4 Rz = {
            {cos(eulerRotation.z),sin(eulerRotation.z),0,0},
            {-sin(eulerRotation.z), cos(eulerRotation.z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
    };
    glm::mat4 S = {
            {scale.x,0,0,0},
            {0, scale.y, 0, 0},
            {0, 0, scale.z, 0},
            {0, 0, 0, 1}
    };

    return  T * Rz * Ry * Rx * S;
}

void Transformation::setTranslation(glm::vec3 newTranslation) {
    translation = newTranslation;
}

void Transformation::setOrientation(glm::quat newOrientation) {
    orientation = newOrientation;
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

glm::quat Transformation::angleAxis(float const& angle, glm::vec3 const& v)
{
    float const a(angle);
    float const s = glm::sin(a * static_cast<float>(0.5));

    return glm::quat(glm::cos(a * static_cast<float>(0.5)), v * s);
}

void Transformation::addRotation(glm::vec3 eulerAngles) {
    auto z = angleAxis(eulerAngles.z, glm::vec3(0,0,1));
    auto y = angleAxis(eulerAngles.y, glm::vec3(0,1,0));
    auto x = angleAxis(eulerAngles.x, glm::vec3(1,0,0));
    orientation *= glm::normalize(x * y * z);
}
