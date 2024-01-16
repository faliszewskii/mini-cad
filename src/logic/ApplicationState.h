//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPLICATIONSTATE_H
#define OPENGL_SANDBOX_APPLICATIONSTATE_H


#include <vector>
#include "importer/AssetImporter.h"
#include "../presentation/scene/nodes/transformation/Transformation.h"
#include "../presentation/scene/nodes/camera/Camera.h"

struct ApplicationState {
    explicit ApplicationState() = default;

    AssetImporter assetImporter;
    std::optional<std::reference_wrapper<SceneNode>> mainFrameBufferNode;
    std::vector<std::unique_ptr<SceneNode>> allNodes;

    std::optional<std::reference_wrapper<SceneNode>> selectedNode;
    std::optional<std::reference_wrapper<Camera>> currentCamera;
};


#endif //OPENGL_SANDBOX_APPLICATIONSTATE_H
