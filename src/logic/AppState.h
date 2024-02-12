//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPSTATE_H
#define OPENGL_SANDBOX_APPSTATE_H


#include <vector>
#include "importer/AssetImporter.h"
#include "../presentation/scene/nodes/transformation/Transformation.h"
#include "../presentation/scene/nodes/camera/Camera.h"
#include "../presentation/scene/tree/StepTree.h"
#include "generator/ModelGenerator.h"

struct AppState {
    explicit AppState() = default;

    AssetImporter assetImporter;
    Scene scene;

    std::optional<std::reference_wrapper<SceneNode>> mainFrameBufferNode; // TODO delete
    std::vector<std::unique_ptr<SceneNode>> allNodes; // TODO delete

    std::optional<std::reference_wrapper<SceneNode>> selectedNode;
    std::optional<std::reference_wrapper<RenderingStep>> selectedStep;
    std::optional<std::reference_wrapper<Camera>> currentCamera;
};


#endif //OPENGL_SANDBOX_APPSTATE_H
