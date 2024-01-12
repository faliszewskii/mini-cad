//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_LIGHT_H
#define OPENGL_SANDBOX_LIGHT_H


#include <glm/vec3.hpp>
#include "../../tree/SceneNode.h"
#include "../../../properties/light/LightTypeProperty.h"
#include "../../../properties/light/PointLightProperty.h"

class Light : public SceneNode {
    std::optional<PointLightProperty> pointLightProperty;
    std::reference_wrapper<LightTypeProperty> currentLightType;
    // TODO rest of light types
public:
    explicit Light(std::string name, glm::vec3 position);

    std::string getTypeName() override { return "Light"; };
    std::vector<std::reference_wrapper<Property>> getProperties() override;

    int acceptVisit(SceneNodeVisitor& visitor) override;
    int acceptLeave(SceneNodeVisitor& visitor) override;
};


#endif //OPENGL_SANDBOX_LIGHT_H
