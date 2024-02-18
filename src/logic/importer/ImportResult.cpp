//
// Created by faliszewskii on 17.02.24.
//

#include "ImportResult.h"
#include "../state/AppState.h"

void ImportResult::addToState(AppState &appState, TransformTree &parent) {
    parent.addChlid(std::move(transformTree));
    appState.materials.insert(appState.materials.end(), std::make_move_iterator(materials.begin()), std::make_move_iterator(materials.end()));
}

void ImportResult::merge(TransformTree &parent, ImportResult &&other) {
    parent.addChlid(std::move(transformTree));
    materials.insert(materials.end(), std::make_move_iterator(other.materials.begin()), std::make_move_iterator(other.materials.end()));
}
