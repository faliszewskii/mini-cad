//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPLICATIONSTATE_H
#define OPENGL_SANDBOX_APPLICATIONSTATE_H


#include <vector>
#include "importer/AssetImporter.h"
#include "../presentation/scene/tree/SceneTreeNode.h"
#include "../presentation/scene/nodes/transformation/Transformation.h"
#include "../presentation/scene/nodes/camera/Camera.h"

struct ApplicationState {
    explicit ApplicationState() :
    rootSceneNode(SceneTreeNode(std::make_unique<Transformation>(Transformation("root")))) {}

    AssetImporter assetImporter;
    SceneTreeNode rootSceneNode;

    std::optional<std::reference_wrapper<SceneNode>> selectedNode;
    std::optional<std::reference_wrapper<Camera>> currentCamera;

    /* TODO
     * Currently all uniforms that will be used have to be set. Find solution for loading an arbitrary shader.
     * Keep in memorywhich uniforms were set so that glGetActiveUniform can check if shader can be displayed.
     */
};


#endif //OPENGL_SANDBOX_APPLICATIONSTATE_H
