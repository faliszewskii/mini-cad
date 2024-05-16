//
// Created by faliszewskii on 12.05.24.
//

#ifndef OPENGL_SANDBOX_SERIALIZER_H
#define OPENGL_SANDBOX_SERIALIZER_H


#include "../state/AppState.h"

class Serializer {


public:
    void importScene(AppState &appState, const std::string &path);

    void exportScene(AppState &appState, const std::string &path);
};


#endif //OPENGL_SANDBOX_SERIALIZER_H
