//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_LIGHT_H
#define OPENGL_SANDBOX_LIGHT_H


#include <glm/vec3.hpp>
#include "../../tree/SceneNode.h"
#include "../../tree/Bindable.h"

class Light : public SceneNode {
    Bindable<glm::vec3> position;
    std::optional<float> attenuation; // TODO
    Bindable<float> strength = 1;
    Bindable<glm::vec3> color = glm::vec3(1.f);
    // TODO rest of light types
public:
    explicit Light(std::string name, glm::vec3 position);

    std::string getTypeName() override { return "Light"; };

    glm::vec3& getPositionRef() { return position.get(); };
    glm::vec3 getPosition() { return position.get(); };

    glm::vec3 &getColorRef() { return color.get(); };
    glm::vec3 getColor() { return color.get(); };

    float &getStrengthRef() { return strength.get(); };
    float getStrength() { return strength.get(); };

    int acceptVisit(SceneNodeVisitor &visitor) override;
    int acceptLeave(SceneNodeVisitor &visitor) override;
};


#endif //OPENGL_SANDBOX_LIGHT_H
