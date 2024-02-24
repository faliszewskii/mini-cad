//
// Created by faliszewskii on 16.01.24.
//

#ifndef OPENGL_SANDBOX_FRAMEBUFFER_H
#define OPENGL_SANDBOX_FRAMEBUFFER_H


#include <utility>

#include "../SceneNode.h"

class FrameBuffer : public SceneNode {
public:
    explicit FrameBuffer(std::string name) : SceneNode(std::move(name)) {}

    std::string getTypeName() override { return "Frame buffer"; };
};


#endif //OPENGL_SANDBOX_FRAMEBUFFER_H
