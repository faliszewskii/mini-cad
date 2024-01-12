//
// Created by faliszewskii on 12.01.24.
//

#ifndef OPENGL_SANDBOX_POINTLIGHTPROPERTY_H
#define OPENGL_SANDBOX_POINTLIGHTPROPERTY_H


#include <string>
#include <glm/vec3.hpp>
#include <optional>
#include "LightTypeProperty.h"

class PointLightProperty : public LightTypeProperty {
    glm::vec3 position;
    std::optional<float> attenuation; // TODO
public:
    PointLightProperty();
    PointLightProperty(glm::vec3 position);

    std::string getPropertyName() override;
    glm::vec3& getPositionRef() { return position; };
    glm::vec3 getPosition() { return position; };

    void acceptVisit(PropertyVisitor& visitor) override;
};


#endif //OPENGL_SANDBOX_POINTLIGHTPROPERTY_H
