//
// Created by faliszewskii on 08.02.24.
//

#ifndef OPENGL_SANDBOX_SCENE_H
#define OPENGL_SANDBOX_SCENE_H

#include <vector>
#include <memory>
#include "../presentation/scene/nodes/transformation/Transformation.h"
#include "../presentation/scene/tree/StepTree.h"


struct Scene {

    #define SCENE_NODE_VECTOR(T, name)                          \
    std::vector<std::unique_ptr<T> > name;                      \
    T& addSceneNode(T &&node) {                                 \
        name.push_back(std::make_unique<T>(std::move(node)));   \
        return *name[name.size()-1];                            \
    }

    SCENE_NODE_VECTOR(FrameBuffer, frameBuffers);
    SCENE_NODE_VECTOR(Transformation, transformations);
    SCENE_NODE_VECTOR(Camera, cameras);
    SCENE_NODE_VECTOR(Material, materials);
    SCENE_NODE_VECTOR(Light, lights);
    SCENE_NODE_VECTOR(Shader, shaders);
    SCENE_NODE_VECTOR(Mesh, meshes);

    std::unique_ptr<StepTree> modelStepRoot;

    explicit Scene() : modelStepRoot(std::make_unique<StepTree>()) {}

    void merge(std::unique_ptr<Scene> &&other, RenderingStep &parent) {
        mergeSceneNodes(frameBuffers, other->frameBuffers);
        mergeSceneNodes(transformations, other->transformations);
        mergeSceneNodes(cameras, other->cameras);
        mergeSceneNodes(materials, other->materials);
        mergeSceneNodes(lights, other->lights);
        mergeSceneNodes(shaders, other->shaders);
        mergeSceneNodes(meshes, other->meshes);
        for(auto &step : other->modelStepRoot->root.getChildren())
            parent.addChild(std::move(step));
    }

    void merge(std::unique_ptr<Scene> &&other) { merge(std::forward<std::unique_ptr<Scene>>(other), modelStepRoot->root); }

private:
    template<typename T>
    void mergeSceneNodes(std::vector<std::unique_ptr<T>> &vector, std::vector<std::unique_ptr<T>> &other) {
        vector.insert(vector.end(), std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
    }
public:

    template<typename T>
    RenderingStep& addStep(T &&step, RenderingStep &parent) { return parent.addChild(std::forward<T>(step)); }

    template<typename T>
    RenderingStep& addStep(T &&step) { return addStep(std::forward<T>(step), modelStepRoot->root); }
};

#endif //OPENGL_SANDBOX_SCENE_H
