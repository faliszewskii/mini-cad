//
// Created by USER on 03/09/2024.
//

#ifndef INTERSECTIONGUI_H
#define INTERSECTIONGUI_H


#include "../../../logic/state/AppState.h"

class IntersectionGui {
    AppState &appState;

    bool isIntersectionEnabled(const std::vector<std::pair<int, EntityType>>& selected);
    std::optional<IntersectableSurface> getIfIntersectableSurface(EntityType entity);
public:
    explicit IntersectionGui(AppState &appState) : appState(appState) {}

    void render();
};



#endif //INTERSECTIONGUI_H
