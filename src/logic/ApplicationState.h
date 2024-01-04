//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPLICATIONSTATE_H
#define OPENGL_SANDBOX_APPLICATIONSTATE_H


#include <vector>
#include "../presentation/rendering/model/ModelNode.h"
#include "importer/AssetImporter.h"

struct ApplicationState {

    explicit ApplicationState(std::vector<Shader> defaultShaders) {
        assert(!defaultShaders.empty());
        this->availableShaders = std::move(defaultShaders);
        globalShader = &availableShaders[0];
    }

    AssetImporter assetImporter;
    ModelNode rootModelNode = ModelNode("root");
    std::vector<ModelNode*> selectedModelNodes;
    std::vector<Shader> availableShaders;
    Shader* globalShader;
    /* TODO
     * Currently all uniforms that will be used have to be set. Find solution for loading an arbitrary shader.
     * Keep in memorywhich uniforms were set so that glGetActiveUniform can check if shader can be displayed.
     */
};


#endif //OPENGL_SANDBOX_APPLICATIONSTATE_H
