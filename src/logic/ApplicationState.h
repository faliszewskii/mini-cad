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

    Camera &setupCamera() {
        auto camera = std::make_unique<Camera>("camera", CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f));
        Camera* p = camera.get();
        rootSceneNode.addChild(std::move(camera));
        return *p;
    }

    explicit ApplicationState(std::vector<Shader> defaultShaders) :
    rootSceneNode(SceneTreeNode(std::make_unique<Transformation>(Transformation("root")))),
    globalShader(defaultShaders[0]), currentCamera(setupCamera()){
        this->availableShaders = std::move(defaultShaders);
    }

    AssetImporter assetImporter;
    SceneTreeNode rootSceneNode;
//    std::vector<ModelNode*> selectedModelNodes;
    std::vector<Shader> availableShaders;
    std::reference_wrapper<Shader> globalShader;
    std::reference_wrapper<Camera> currentCamera;
    /* TODO
     * Currently all uniforms that will be used have to be set. Find solution for loading an arbitrary shader.
     * Keep in memorywhich uniforms were set so that glGetActiveUniform can check if shader can be displayed.
     */
};


#endif //OPENGL_SANDBOX_APPLICATIONSTATE_H
