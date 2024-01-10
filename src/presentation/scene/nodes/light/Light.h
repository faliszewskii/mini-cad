//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_LIGHT_H
#define OPENGL_SANDBOX_LIGHT_H


#include "../../tree/SceneNode.h"

class Light : public SceneNode {
protected:
    Light() = default;
public:
    int acceptVisit(SceneNodeVisitor& visitor) override;
    int acceptLeave(SceneNodeVisitor& visitor) override;
};


#endif //OPENGL_SANDBOX_LIGHT_H
