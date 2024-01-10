//
// Created by faliszewskii on 07.01.24.
//

#include "Transformation.h"

#include <utility>


Transformation::Transformation(std::string name) : SceneNode(std::move(name)), transformation(glm::mat4(1.0f)) {}

Transformation::Transformation(std::string name, glm::mat4 transformation) : SceneNode(std::move(name)), transformation(transformation) {

}

int Transformation::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitTransformation(*this);
}

int Transformation::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveTransformation(*this);
}
