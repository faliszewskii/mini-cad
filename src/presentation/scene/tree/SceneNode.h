//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_SCENENODE_H
#define OPENGL_SANDBOX_SCENENODE_H


#define UUID_SYSTEM_GENERATOR
#include "../../../../lib/uuid/uuid.h"
#include "../visitor/SceneNodeVisitor.h"
#include <string>


class SceneNode {
    uuids::uuid uniqueObjectId;
    std::string name;
public:
    explicit SceneNode(std::string name) : name(std::move(name)) {
        uniqueObjectId = uuids::uuid_system_generator{}();
    }
    uuids::uuid getUuid() { return uniqueObjectId; };
    std::string getName() { return name; };

    virtual int acceptVisit(SceneNodeVisitor& visitor) = 0;
    virtual int acceptLeave(SceneNodeVisitor& visitor) = 0;

    virtual ~SceneNode() = default;
protected:
    SceneNode() = default;
};


#endif //OPENGL_SANDBOX_SCENENODE_H
