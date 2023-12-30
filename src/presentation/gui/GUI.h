//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_GUI_H
#define OPENGL_SANDBOX_GUI_H

#include "../../logic/ApplicationState.h"
#include "imgui.h"

struct GUIApplicationState {
    ModelNode &rootModelNode;
    std::vector<ModelNode*> &selectedModelNodes;
};

class GUI {
public:
    explicit GUI(GLFWwindow *window, ApplicationState& state);
    void render();
    void newFrame();
private:
    GUIApplicationState guiState;

    void renderMenuBar() const;

    void renderDebugOverlay();

    void renderMainWindow();

    void renderModelView();

    void traverseModeNode(ModelNode &node, ImGuiTreeNodeFlags flags);
};


#endif //OPENGL_SANDBOX_GUI_H
