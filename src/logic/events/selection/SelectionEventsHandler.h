//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H
#define OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H

#include "../../state/AppState.h"

namespace SelectionEventsHandler {

    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;
        eventPublisher.subscribe([&](const SelectEntityEvent &event) {
            auto &set = appState.selectedEntities;
            if (!appState.keyboardCtrlMode) {
                for(auto &el : set) std::visit([](auto &el){el.get().selected = false;}, el.second);
                set.clear();
            } // TODO what if object is already selected and I have ctrl
            std::visit(overloaded{
                    [&](auto &el){
                        auto &entity = el.get();
                        {
                            set.emplace_back(entity.id, entity);
                            entity.selected = true;
                        }
                    }
            }, event.selected);
            eventPublisher.publish(SelectionChangedEvent{});
        });

        eventPublisher.subscribe([&](const SelectionChangedEvent &event) {
            // Center of mass on Selection Changed.
            auto &set = appState.selectedEntities;
            if(set.size() > 1) {
                glm::vec3 center = glm::vec3();
                int n = 0;
                for(auto &el : set) {
                    std::visit(overloaded{
                            [&](Torus &torus) { center += torus.transform.translation; n++; },
                            [&](Point &point) { center += point.position; n++; },
                            [&](BezierC0 &bezier) { /* ignore */ },
                            [&](BezierC2 &bezier) { /* ignore */ }
                    }, el.second);
                }
                center /= n;
                appState.centerOfMassTransformation = Transformation{center};
            }
        });

        eventPublisher.subscribe([&](const SelectionChangedEvent &event) {
            // Reset Bezier subselection
            appState.selectedBezierPoints.clear();
        });
    }
}

#endif //OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H
