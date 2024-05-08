//
// Created by faliszewskii on 03.05.24.
//

#ifndef OPENGL_SANDBOX_BEZIERPATCHCREATORUI_H
#define OPENGL_SANDBOX_BEZIERPATCHCREATORUI_H


#include "../../../logic/state/AppState.h"

class BezierPatchCreatorUI {
    AppState &appState;
public:
    explicit BezierPatchCreatorUI(AppState &appState) : appState(appState) {}

    void render();
};


#endif //OPENGL_SANDBOX_BEZIERPATCHCREATORUI_H
