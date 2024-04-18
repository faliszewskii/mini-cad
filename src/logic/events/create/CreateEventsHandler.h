//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_CREATEEVENTSHANDLER_H
#define OPENGL_SANDBOX_CREATEEVENTSHANDLER_H

#include "../../state/AppState.h"

namespace CreateEventsHandler {

    template<typename T>
    inline void addPointToCurve(Point &point, EntityType &curveET) {
        if(holds_alternative<std::reference_wrapper<T>>(curveET)) {
            T &curve = std::get<std::reference_wrapper<T>>(curveET);
            curve.addPoint(point);
        }
    }

    template<typename T>
    inline void createCurve(AppState &appState, std::map<int, std::unique_ptr<T>> &map) {
        auto &selectedEntities = appState.selectedEntities;
        auto curve = std::make_unique<T>();
        auto result = map.emplace(curve->id, std::move(curve));

        // Add all currently selected points;
        for(auto &el : selectedEntities) { // TODO Consider separating selected entities into object types.
            if(holds_alternative<std::reference_wrapper<Point>>(el.second)) {
                Point &point = std::get<std::reference_wrapper<Point>>(el.second);
                result.first->second->addPoint(point);
            }
        }
    }

    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;
        auto &selectedEntities = appState.selectedEntities;

        eventPublisher.subscribe([&](const CreateTorusEvent &event){
            auto torus = std::make_unique<Torus>(event.position);
            appState.torusSet.emplace(torus->id, std::move(torus));
        });

        eventPublisher.subscribe([&](const CreatePointEvent &event) {
            auto pointTemp = std::make_unique<Point>(event.position);
            auto result = appState.pointSet.emplace(pointTemp->id, std::move(pointTemp));
            auto &point = *result.first->second;
            if(!selectedEntities.empty()) { // TODO Move this to entirely different handler for bezier that is called on PointCreatedEvent.
                auto &last = (selectedEntities.rbegin())->second;
                addPointToCurve<BezierC0>(point, last);
                addPointToCurve<BezierC2>(point, last);
                addPointToCurve<InterpolatedC2>(point, last);
            }
        });

        eventPublisher.subscribe([&](const CreateBezierC0Event &event) {
            createCurve(appState, appState.bezierC0Set);
        });

        eventPublisher.subscribe([&](const CreateBezierC2Event &event) {
            createCurve(appState, appState.bezierC2Set);
        });

        eventPublisher.subscribe([&](const CreateInterpolatedC2Event &event) {
            createCurve(appState, appState.interpolatedC2Set);
        });
    }
}

#endif //OPENGL_SANDBOX_CREATEEVENTSHANDLER_H
