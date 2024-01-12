//
// Created by faliszewskii on 10.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATIONPROPERTY_H
#define OPENGL_SANDBOX_TRANSFORMATIONPROPERTY_H


#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include "../Property.h"

class TransformationProperty : public Property {
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

public:
    TransformationProperty();
    TransformationProperty(glm::vec3 position, glm::quat orientation, glm::vec3 scale);
    std::string getPropertyName() override { return "Transformation"; };

    glm::mat4 getTransformation();

    void setPosition(glm::vec3 newPosition);
    void setOrientation(glm::quat newOrientation);
    void setOrientation(glm::vec3 eulerAngles);
    void setScale(glm::vec3 newScale);

    glm::vec3& getPositionRef() { return position; };
    glm::quat& getOrientationRef() { return orientation; };
    glm::vec3& getScaleRef() { return scale; };

    void acceptVisit(PropertyVisitor& visitor) override {
        visitor.visitTransformationProperty(*this);
    }

};


#endif //OPENGL_SANDBOX_TRANSFORMATIONPROPERTY_H
