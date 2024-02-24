//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_SCENENODE_H
#define OPENGL_SANDBOX_SCENENODE_H


#define UUID_SYSTEM_GENERATOR
#include "../../../lib/uuid/uuid.h"
#include <string>
#include <stack>
#include <ranges>
#include <iostream>

class SceneNode {
    uuids::uuid uniqueObjectId;
    std::string name;
public:
    explicit SceneNode(std::string name) : name(std::move(name)) {
        uniqueObjectId = uuids::uuid_system_generator{}();
    }

    [[nodiscard]] uuids::uuid getUuid() const { return uniqueObjectId; };
    [[nodiscard]] std::string getName() const { return name; };
    virtual std::string getTypeName() = 0;

    virtual ~SceneNode() = default;
    SceneNode (const SceneNode&) = delete;
    SceneNode& operator= (const SceneNode&) = delete;
    SceneNode(SceneNode &&other) noexcept = default;
    SceneNode& operator=(SceneNode &&other) = default;

protected:
    SceneNode() = default;
};


#endif //OPENGL_SANDBOX_SCENENODE_H
