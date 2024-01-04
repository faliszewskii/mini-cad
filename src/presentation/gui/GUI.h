//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_GUI_H
#define OPENGL_SANDBOX_GUI_H

#include "../../logic/ApplicationState.h"
#include "imgui.h"

struct ViewsMask {
    static constexpr unsigned int ModelsView = 1 << 0;
    static constexpr unsigned int ShadersView = 1 << 1;
    ViewsMask() = delete;
};

struct GUIApplicationState {

    explicit GUIApplicationState(ApplicationState& appState) : rootModelNode(appState.rootModelNode),
    selectedModelNodes(appState.selectedModelNodes), assetImporter(appState.assetImporter),
    availableShaders(appState.availableShaders), globalShader(appState.globalShader) {
        activeViewsMask = ViewsMask::ModelsView | ViewsMask::ShadersView; // TODO Get from last user settings.
    }
    // ApplicationState
    AssetImporter &assetImporter;
    ModelNode &rootModelNode;
    std::vector<ModelNode*> &selectedModelNodes;
    std::vector<Shader> &availableShaders;
    Shader* &globalShader;
    // this
    unsigned int activeViewsMask;

};

class GUI {
public:
    explicit GUI(GLFWwindow *window, ApplicationState& state);
    void render();
    void newFrame();
private:
    GUIApplicationState guiState;

    void renderMenuBar();

    void renderDebugOverlay();

    void renderModelWindow();

    void renderModelTreeView();

    void traverseModelNode(ModelNode &node, ImGuiTreeNodeFlags flags);

    void renderShaderWindow();

    void renderShaderListView();
};


#endif //OPENGL_SANDBOX_GUI_H
