//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMATION_H
#define OPENGL_SANDBOX_TRANSFORMATION_H


#include <glm/glm.hpp>
#include "../SceneNode.h"
#include <glm/ext/quaternion_float.hpp>

class Transformation {
public: // TODO Refactor whole
    glm::vec<3, double> translation;
    glm::qua<double> orientation;
    glm::vec<3, double> scale;

    explicit Transformation();
    explicit Transformation(glm::mat<4,4,double> transformation);
    explicit Transformation(glm::vec<3, double> position, glm::qua<double> orientation = glm::qua<double>(1,0,0,0),
                   glm::vec<3, double> scale = glm::vec<3, double>(1.0f));

    [[nodiscard]] glm::mat<4,4,double> getTransformation() const;
    void setTransformation(glm::mat<4,4,double> transformation);

    void setTranslation(glm::vec<3, double> newTranslation);
    void setOrientation(glm::qua<double> newOrientation);
    void addRotation(glm::vec<3, double> eulerAngles);
    void setScale(glm::vec<3, double> newScale);

    glm::vec<3, double> &getTranslationRef() { return translation; };
    glm::qua<double> &getOrientationRef() { return orientation; };
    glm::vec<3, double> &getScaleRef() { return scale; };

    [[nodiscard]] glm::vec<3, double> getRotationAngles() const;

    void setRotation(glm::vec<3, double> rotation);
};


#endif //OPENGL_SANDBOX_TRANSFORMATION_H
