//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_GUI_H
#define OPENGL_SANDBOX_GUI_H

#include "../../logic/ApplicationState.h"
#include "imgui.h"

struct ViewsMask {
    static constexpr unsigned int MainView = 1 << 0;
    static constexpr unsigned int ShadersView = 1 << 1;
    ViewsMask() = delete;
};

struct GUIApplicationState {

    explicit GUIApplicationState(ApplicationState& appState) : assetImporter(appState.assetImporter),
    rootSceneNode(appState.rootSceneNode), selectedNode(appState.selectedNode), selectedProperty(0), guiWidth(300) {
        activeViewsMask = ViewsMask::MainView | ViewsMask::ShadersView; // TODO Get from last user settings.
    }
    // ApplicationState
    AssetImporter &assetImporter;
    std::optional<std::reference_wrapper<SceneNode>>& selectedNode;
    SceneTreeNode &rootSceneNode;
    // this
    unsigned int activeViewsMask;
    int selectedProperty;
    int guiWidth;

};

class GUI {
public:
    explicit GUI(GLFWwindow *window, ApplicationState& state);
    void render();
    void newFrame();
    int getGuiWidth() const { return guiState.guiWidth; };
    ~GUI();
private:
    GUIApplicationState guiState;


    void renderMenuBar();

    void renderDebugOverlay();

    void renderMainWindow();

    void renderModelTreeView();

    void renderShaderWindow();

    void renderShaderListView();
};


#endif //OPENGL_SANDBOX_GUI_H
