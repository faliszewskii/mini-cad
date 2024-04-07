//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H
#define OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H

#include <variant>
#include "../../state/AppState.h"

namespace SelectionEventsHandler {

    void resetSelection(std::vector<std::pair<int, EntityType>> &set);

    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;
        eventPublisher.subscribe([&](const SelectEntityEvent &event) {
            auto &set = appState.selectedEntities;
            if (!appState.keyboardCtrlMode) { // TODO what if object is already selected and I have ctrl
                if(event.contextLevel > appState.selectionContext.size()) // Higher level context
                    appState.selectionContext.emplace_back(appState.selectedEntities[0]);
                while(event.contextLevel < appState.selectionContext.size())
                    appState.selectionContext.pop_back();
                resetSelection(set);
            }
            if(!set.empty())
                std::visit(overloaded{ // Check if selected element is a virtual point.
                    [&](auto &el) {
                        for(auto &bezier : appState.bezierC2Set)
                            for(auto &bernsteinPoint : bezier.second->bernsteinPoints)
                                if(bernsteinPoint->id ==  el.get().id) resetSelection(set);
                    }
                }, set[0].second);
            std::visit(overloaded{
                    [&](auto &el){
                        auto &entity = el.get();
                        set.emplace_back(entity.id, entity);
                        entity.selected = true;
                    },
                    [&](const std::reference_wrapper<Point> &pointRef){ // Point has to be checked against being a virtual point. We don't want to select multiple of those.
                        auto &point = pointRef.get();
                        for(auto &bezier : appState.bezierC2Set) {
                            for(auto &bernsteinPoint : bezier.second->bernsteinPoints) {
                                // Check if the point to be selected is a virtual point
                                if(bernsteinPoint->id == point.id)
                                    resetSelection(set);
                            }
                        }
                        set.emplace_back(point.id, point);
                        point.selected = true;
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
    }

    void resetSelection(std::vector<std::pair<int, EntityType>> &set) {
        for(auto &el : set) std::visit([](auto &el){ el.get().selected = false;}, el.second);
        set.clear();
    }
}

#endif //OPENGL_SANDBOX_SELECTIONEVENTSHANDLER_H
