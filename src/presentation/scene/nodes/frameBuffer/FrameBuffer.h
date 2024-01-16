//
// Created by faliszewskii on 16.01.24.
//

#ifndef OPENGL_SANDBOX_FRAMEBUFFER_H
#define OPENGL_SANDBOX_FRAMEBUFFER_H


#include <utility>

#include "../../tree/SceneNode.h"

class FrameBuffer : public SceneNode {
public:
    explicit FrameBuffer(std::string name) : SceneNode(std::move(name)) {}
    std::string getTypeName() override { return "Frame buffer"; };

    int acceptVisit(SceneNodeVisitor &visitor) override {
        visitor.visitFrameBuffer(*this);
    };

    int acceptLeave(SceneNodeVisitor &visitor) override {
        visitor.leaveFrameBuffer(*this);
    }

};


#endif //OPENGL_SANDBOX_FRAMEBUFFER_H
