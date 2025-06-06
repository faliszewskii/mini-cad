//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATION_H
#define OPENGL_SANDBOX_TRANSFORMATION_H


#include <glm/glm.hpp>
#include "../SceneNode.h"
#include <glm/ext/quaternion_float.hpp>

class Transformation {
public:
    glm::vec3 translation;
    glm::quat orientation;
    glm::vec3 scale;

    explicit Transformation();
    explicit Transformation(glm::vec3 position, glm::quat orientation = glm::quat(1,0,0,0),
                   glm::vec3 scale = glm::vec3(1.0f));

    [[nodiscard]] glm::mat4 getTransformation() const;

    void setTranslation(glm::vec3 newTranslation);
    void setOrientation(glm::quat newOrientation);
    void addRotation(glm::vec3 eulerAngles);
    void setScale(glm::vec3 newScale);

    glm::vec3 &getTranslationRef() { return translation; };
    glm::quat &getOrientationRef() { return orientation; };
    glm::vec3 &getScaleRef() { return scale; };

    [[nodiscard]] glm::vec3 getRotationAngles() const;

    void setRotation(glm::vec3 rotation);

    glm::quat angleAxis(float const& angle, glm::vec3 const& v);
};


#endif //OPENGL_SANDBOX_TRANSFORMATION_H
