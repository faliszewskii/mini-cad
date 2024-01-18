//
// Created by faliszewskii on 17.01.24.
//

#ifndef OPENGL_SANDBOX_EDITORNODEVISITOR_H
#define OPENGL_SANDBOX_EDITORNODEVISITOR_H


#include "../../scene/visitor/SceneNodeVisitor.h"
#include "../../scene/tree/SceneNode.h"
#include "../node/NodeDetailsVisitor.h"

class EditorNodeVisitor : public SceneNodeVisitor {
    NodeDetailsVisitor nodeDetailsVisitor;
    int uniqueId = 0;
    int visitFrameBuffer(FrameBuffer &frameBuffer) override;
    int visitTransformation(Transformation &transformation) override;
    int visitMesh(Mesh &mesh) override;
    int visitLight(Light &light) override;
    int visitCamera(Camera &camera) override;
    int visitShader(Shader &shader) override;
    int visitMaterial(Material &material) override;

    int drawNode(SceneNode &node);
};


#endif //OPENGL_SANDBOX_EDITORNODEVISITOR_H
