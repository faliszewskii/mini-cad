//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATION_H
#define OPENGL_SANDBOX_TRANSFORMATION_H


#include <glm/glm.hpp>
#include "../../tree/SceneNode.h"
#include "../../../properties/transformation/TransformationProperty.h"

class Transformation : public SceneNode {
    TransformationProperty transformationProperty;
public:
    explicit Transformation(std::string name);
    Transformation(std::string name, glm::mat4 transformation);
    Transformation(std::string name, glm::vec3 position, glm::quat orientation, glm::vec3 scale);

    glm::mat4 getTransformation() { return transformationProperty.getTransformation(); };
    std::string getTypeName() override { return "Transformation"; };

    std::vector<std::reference_wrapper<Property>> getProperties() override;
    int acceptVisit(SceneNodeVisitor& visitor) override;
    int acceptLeave(SceneNodeVisitor& visitor) override;
};


#endif //OPENGL_SANDBOX_TRANSFORMATION_H
