//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_CREATEEVENTSHANDLER_H
#define OPENGL_SANDBOX_CREATEEVENTSHANDLER_H

#include "../../state/AppState.h"

namespace CreateEventsHandler {
    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;
        auto &selectedEntities = appState.selectedEntities;

        eventPublisher.subscribe([&](const CreateTorusEvent &event){
            auto torus = std::make_unique<Torus>(event.position);
            appState.torusSet.emplace(torus->id, std::move(torus));
        });

        eventPublisher.subscribe([&](const CreatePointEvent &event) {
            auto point = std::make_unique<Point>(event.position);
            auto result = appState.pointSet.emplace(point->id, std::move(point));
            if(!selectedEntities.empty()) { // TODO Move this to entirely different handler for bezier that is called on PointCreatedEvent.
                auto &last = (selectedEntities.rbegin())->second;
                if(holds_alternative<std::reference_wrapper<BezierC0>>(last)) {
                    BezierC0 &bezier = std::get<std::reference_wrapper<BezierC0>>(last);
                    bezier.addPoint(*result.first->second);
                }
                if(holds_alternative<std::reference_wrapper<BezierC2>>(last)) { // TODO Extract or common bezier base.
                    BezierC2 &bezier = std::get<std::reference_wrapper<BezierC2>>(last);
                    bezier.addPoint(*result.first->second);
                }
            }
        });

        eventPublisher.subscribe([&](const CreateBezierC0Event &event) {
            auto bezier = std::make_unique<BezierC0>();
            auto result = appState.bezierC0Set.emplace(bezier->id, std::move(bezier));

            // Add all currently selected points;
            for(auto &el : selectedEntities) { // TODO Consider separating selected entities into object types.
                if(holds_alternative<std::reference_wrapper<Point>>(el.second)) {
                    Point &point = std::get<std::reference_wrapper<Point>>(el.second);
                    result.first->second->addPoint(point);
                }
            }
        });

        eventPublisher.subscribe([&](const CreateBezierC2Event &event) {
            auto bezier = std::make_unique<BezierC2>();
            auto result = appState.bezierC2Set.emplace(bezier->id, std::move(bezier));

            // Add all currently selected points;
            for(auto &el : selectedEntities) {
                if(holds_alternative<std::reference_wrapper<Point>>(el.second)) {
                    Point &point = std::get<std::reference_wrapper<Point>>(el.second);
                    result.first->second->addPoint(point);
                }
            }
        });
    }
}

#endif //OPENGL_SANDBOX_CREATEEVENTSHANDLER_H
