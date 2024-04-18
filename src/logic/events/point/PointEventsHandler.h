//
// Created by faliszewskii on 21.03.24.
//

#ifndef OPENGL_SANDBOX_POINTEVENTSHANDLER_H
#define OPENGL_SANDBOX_POINTEVENTSHANDLER_H

#include "../../state/AppState.h"

namespace PointEventsHandler {
    void setup(AppState &appState) {
        auto &eventPublisher = appState.eventPublisher;

        eventPublisher.subscribe([&](const PointMovedEvent &event) {
            // TODO There should be a data structure that would allow to query for Bezier that is build from given point.
            for(auto &bezier : appState.bezierC0Set) {
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.point.id)
                        bezier.second->updatePoint(event.point, i);
            }
            for(auto &bezier : appState.bezierC2Set) {
                // TODO Code duplication with above. It is hard to keep track of what is needed to be changed when adding new geometry.
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.point.id)
                        bezier.second->updatePoint(event.point, i);
                for(int i = 0; i < bezier.second->bernsteinPoints.size(); i++)
                    if(bezier.second->bernsteinPoints[i]->id == event.point.id)
                        bezier.second->updateBernstein(event.point, i, event.delta, eventPublisher);
            }
            for(auto &interpolated : appState.interpolatedC2Set) { // TODO This just asks to be extracted.
                for (int i = 0; i < interpolated.second->controlPoints.size(); i++)
                    if (interpolated.second->controlPoints[i].first == event.point.id)
                        interpolated.second->updatePoint(event.point, i);
            }
        });

        eventPublisher.subscribe([&](const PointDeletedEvent &event) {
            // TODO There should be a data structure that would allow to query for Bezier that is build from given point.
            for(auto &bezier : appState.bezierC0Set) {
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.id)
                        bezier.second->removePoint(i);
            }
            for(auto &bezier : appState.bezierC2Set) { // TODO Extract to function or make bezier inherit from common base.
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.id)
                        bezier.second->removePoint(i);
            }
            for(auto &interpolated : appState.interpolatedC2Set) {
                for(int i = 0; i < interpolated.second->controlPoints.size(); i++)
                    if(interpolated.second->controlPoints[i].first == event.id)
                        interpolated.second->removePoint(i);
            }
        });
    }
}
#endif //OPENGL_SANDBOX_POINTEVENTSHANDLER_H
