//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_GUI_H
#define OPENGL_SANDBOX_GUI_H

#include "../../logic/state/AppState.h"
#include "../../../lib/imgui-node-editor/imgui_node_editor.h"
#include "imgui.h"
#include "creator/BezierPatchCreatorUI.h"

class GUI {
    AppState &appState;
    BezierPatchCreatorUI bezierPatchCreatorUi;
public:
    explicit GUI(AppState &appState, GLFWwindow *window);

    void render();

    static void newFrame();

    ~GUI();
};


#endif //OPENGL_SANDBOX_GUI_H
