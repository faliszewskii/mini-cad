//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATION_H
#define OPENGL_SANDBOX_TRANSFORMATION_H


#include <glm/glm.hpp>
#include "../../tree/SceneNode.h"

class Transformation : public SceneNode {
    glm::mat4 transformation;
public:
    explicit Transformation(std::string name);
    Transformation(std::string name, glm::mat4 transformation);
    glm::mat4 getTransformation() { return transformation; };

    int acceptVisit(SceneNodeVisitor& visitor) override;
    int acceptLeave(SceneNodeVisitor& visitor) override;
};


#endif //OPENGL_SANDBOX_TRANSFORMATION_H
