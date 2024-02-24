//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_IMPORTRESULT_H
#define OPENGL_SANDBOX_IMPORTRESULT_H

#include <vector>
#include <memory>
#include "../state/TransformTree.h"

class AppState;

struct ImportResult {
    std::unique_ptr<TransformTree> transformTree;
    std::vector<std::unique_ptr<Material>> materials;

    void merge(TransformTree &parent, ImportResult &&other);

    void addToState(AppState &appState, TransformTree &parent);
};

#endif //OPENGL_SANDBOX_IMPORTRESULT_H
