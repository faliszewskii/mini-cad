//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_GUI_H
#define OPENGL_SANDBOX_GUI_H

#include "../../logic/ApplicationState.h"
#include "../../../lib/imgui-node-editor/imgui_node_editor.h"
#include "imgui.h"

struct ViewsMask {
    static constexpr unsigned int MainView = 1 << 0;
    static constexpr unsigned int ShadersView = 1 << 1;

    ViewsMask() = delete;
};

struct GUIApplicationState {

    explicit GUIApplicationState(ApplicationState &appState) : assetImporter(appState.assetImporter),
                                                               currentCamera(appState.currentCamera),
                                                               mainFrameBufferNode(appState.mainFrameBufferNode),
                                                               selectedNode(appState.selectedNode),
                                                               guiWidth(300),
                                                               allNodes(appState.allNodes) {
        activeViewsMask = ViewsMask::MainView | ViewsMask::ShadersView; // TODO Get from last user settings.
    }

    // ApplicationState
    AssetImporter &assetImporter;
    std::optional<std::reference_wrapper<SceneNode>> &mainFrameBufferNode;
    std::vector<std::unique_ptr<SceneNode>> &allNodes;
    std::optional<std::reference_wrapper<SceneNode>> &selectedNode;
    std::optional<std::reference_wrapper<Camera>> &currentCamera;
    // this
    unsigned int activeViewsMask;
    int guiWidth;

};

class GUI {
public:
    explicit GUI(GLFWwindow *window, ApplicationState &state);

    void render();

    void newFrame();

    int getGuiWidth() const { return guiState.guiWidth; };

    ~GUI();

private:
    GUIApplicationState guiState;
    ax::NodeEditor::EditorContext *m_Context = nullptr;
    bool firstTime = true;

    void renderMenuBar();

    void renderDebugOverlay();

    void renderMainWindow();

    void renderEditorWindow();

    void renderModelTreeView();

    void renderLogOverlay();

    void renderGizmo();

    void renderMenuItemLoadModel();

    void addModel(std::unique_ptr<SceneNode> &&model);
    void addModel(std::vector<std::unique_ptr<SceneNode>> &&model);
};


#endif //OPENGL_SANDBOX_GUI_H
