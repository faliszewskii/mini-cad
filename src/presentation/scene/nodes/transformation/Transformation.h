//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATION_H
#define OPENGL_SANDBOX_TRANSFORMATION_H


#include <glm/glm.hpp>
#include "../../tree/SceneNode.h"
#include "../../tree/Bindable.h"
#include <glm/ext/quaternion_float.hpp>

class Transformation : public SceneNode {
public:
    Bindable<glm::vec3> translation;
    Bindable<glm::quat> orientation;
    Bindable<glm::vec3> scale;

    explicit Transformation(std::string name);
    Transformation(std::string name, glm::mat4 transformation);
    Transformation(std::string name, Bindable<glm::vec3> position, Bindable<glm::quat> orientation = glm::quat(1,0,0,0),
                   Bindable<glm::vec3> scale = glm::vec3(1.0f));

    std::string getTypeName() override { return "Transformation"; };

    glm::mat4 getTransformation();
    void setTransformation(glm::mat4 transformation);

    void setTranslation(glm::vec3 newTranslation);
    void setOrientation(glm::quat newOrientation);
    void setOrientation(glm::vec3 eulerAngles);
    void setScale(glm::vec3 newScale);

    glm::vec3 &getTranslationRef() { return translation.get(); };
    glm::quat &getOrientationRef() { return orientation.get(); };
    glm::vec3 &getScaleRef() { return scale.get(); };

    int acceptVisit(SceneNodeVisitor &visitor) override;
    int acceptLeave(SceneNodeVisitor &visitor) override;
};


#endif //OPENGL_SANDBOX_TRANSFORMATION_H
