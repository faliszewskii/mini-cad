//
// Created by faliszewskii on 12.01.24.
//

#ifndef OPENGL_SANDBOX_POINTLIGHTPROPERTY_H
#define OPENGL_SANDBOX_POINTLIGHTPROPERTY_H


#include <string>
#include <glm/vec3.hpp>
#include <optional>
#include "LightTypeProperty.h"
#include "../../scene/tree/Bindable.h"

class PointLightProperty : public LightTypeProperty {
    Bindable<glm::vec3> position;
    std::optional<float> attenuation; // TODO
public:
    explicit PointLightProperty(Bindable<glm::vec3> position);

    std::string getPropertyName() override;

    glm::vec3 &getPositionRef() { return position.get(); };
    glm::vec3 getPosition() { return position.get(); };

    void acceptVisit(PropertyVisitor &visitor) override;
};


#endif //OPENGL_SANDBOX_POINTLIGHTPROPERTY_H
