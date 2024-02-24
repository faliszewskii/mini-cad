//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_LIGHT_H
#define OPENGL_SANDBOX_LIGHT_H


#include <glm/vec3.hpp>
#include "../SceneNode.h"

class Light : public SceneNode {
public:
    glm::vec3 position;
    float constantAttenuation = 1;
    float linearAttenuation = 0.045;
    float quadraticAttenuation = 0.075;
    float strength = 1;
    glm::vec3 color = glm::vec3(1.f);
    // TODO rest of light types
public:
    explicit Light(std::string name, glm::vec3 position);

    std::string getTypeName() override { return "Light"; };

    glm::vec3& getPositionRef() { return position; };
    [[nodiscard]] glm::vec3 getPosition() const { return position; };

    glm::vec3& getColorRef() { return color; };
    [[nodiscard]] glm::vec3 getColor() const { return color; };

    float& getStrengthRef() { return strength; };
    [[nodiscard]] float getStrength() const { return strength; };

    float& getConstantAttenuationRef() { return constantAttenuation; };
    [[nodiscard]] float getConstantAttenuation() const { return constantAttenuation; };
    float& getLinearAttenuationRef() { return linearAttenuation; };
    [[nodiscard]] float getLinearAttenuation() const { return linearAttenuation; };
    float& getQuadraticAttenuationRef() { return quadraticAttenuation; };
    [[nodiscard]] float getQuadraticAttenuation() const { return quadraticAttenuation; };
};


#endif //OPENGL_SANDBOX_LIGHT_H
