//
// Created by faliszewskii on 07.01.24.
//

#include "Light.h"

int Light::acceptVisit(SceneNodeVisitor &visitor) {
    return visitor.visitLight(*this);
}

int Light::acceptLeave(SceneNodeVisitor &visitor) {
    return visitor.leaveLight(*this);
}