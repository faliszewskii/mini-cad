//
// Created by faliszewskii on 29.05.24.
//

#ifndef OPENGL_SANDBOX_GREGORYPATCHCREATORUI_H
#define OPENGL_SANDBOX_GREGORYPATCHCREATORUI_H


#include "../../../logic/state/AppState.h"

class GregoryPatchCreatorUI {
    AppState &appState;

    int selectedHole = -1;
public:
    explicit GregoryPatchCreatorUI(AppState &appState) : appState(appState) {}

    void render();
};


#endif //OPENGL_SANDBOX_GREGORYPATCHCREATORUI_H
