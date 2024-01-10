//
// Created by faliszewskii on 07.01.24.
//

#include "PointLight.h"



int PointLight::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitPointLight(*this);
}

int PointLight::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leavePointLight(*this);
}
