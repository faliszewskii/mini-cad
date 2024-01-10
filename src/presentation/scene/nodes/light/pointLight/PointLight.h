//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_POINTLIGHT_H
#define OPENGL_SANDBOX_POINTLIGHT_H


#include "../Light.h"

class PointLight : public Light {
public:
    int acceptVisit(SceneNodeVisitor &visitor) override;
    int acceptLeave(SceneNodeVisitor &visitor) override;
};


#endif //OPENGL_SANDBOX_POINTLIGHT_H
