//
// Created by faliszewskii on 07.01.24.
//
#include "Transformation.h"
#include "../../../../logic/algebra/AlgebraUtils.h"

#include <utility>


Transformation::Transformation(std::string name) : SceneNode(std::move(name)), transformationProperty() {}

Transformation::Transformation(std::string name, Bindable<glm::vec3> position, glm::quat orientation, glm::vec3 scale) :
        SceneNode(std::move(name)), transformationProperty(std::move(position), orientation, scale) {}

Transformation::Transformation(std::string name, glm::mat4 transformation): SceneNode(std::move(name)) {
    glm::vec3 scale; glm::quat rotation; glm::vec3 translation;
    AlgebraUtils::decomposeMtx(transformation, translation, rotation, scale);
    transformationProperty.setPosition(translation);
    transformationProperty.setOrientation(rotation);
    transformationProperty.setScale(scale);
}

int Transformation::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitTransformation(*this);
}

int Transformation::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveTransformation(*this);
}

std::vector<std::reference_wrapper<Property>> Transformation::getProperties() {
    std::vector<std::reference_wrapper<Property>> vec{transformationProperty};
    return vec;
}

void Transformation::setTransformation(glm::mat4 transformation) {
    glm::vec3 scale; glm::quat rotation; glm::vec3 translation;
    AlgebraUtils::decomposeMtx(transformation, translation, rotation, scale);
    transformationProperty.setPosition(translation);
    transformationProperty.setOrientation(rotation);
    transformationProperty.setScale(scale);
}
