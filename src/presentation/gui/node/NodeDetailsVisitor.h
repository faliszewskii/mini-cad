//
// Created by faliszewskii on 11.01.24.
//

#ifndef OPENGL_SANDBOX_NODEDETAILSVISITOR_H
#define OPENGL_SANDBOX_NODEDETAILSVISITOR_H


#include <functional>
#include <optional>
#include "../../scene/visitor/SceneNodeVisitor.h"

class NodeDetailsVisitor : public SceneNodeVisitor {
public:
    int visitShader(Shader &shader) override;
    int visitLight(Light &light) override;
    int visitMaterial(Material &material) override;
};


#endif //OPENGL_SANDBOX_NODEDETAILSVISITOR_H
