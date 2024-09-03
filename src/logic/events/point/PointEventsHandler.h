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
            for(auto &patch : appState.patchC0Set) {
                bool patchChanged = false;
                for (int i = 0; i < patch.second->controlPoints.size(); i++)
                    if (patch.second->controlPoints[i].first == event.point.id) {
                        patchChanged = true;
                        patch.second->updatePoint(event.point, i);
                    }
                if (patchChanged) {
                    for(auto &pIntersection : appState.intersectionSet) {
                        Intersection &intersection = *pIntersection.second;
                        if(std::visit(overloaded{[&](auto &e){ return e.get().id == patch.first;}}, intersection.surfaces[0].first) ||
                            std::visit(overloaded{[&](auto &e){ return e.get().id == patch.first;}}, intersection.surfaces[1].first)) {
                            for(auto &pSurface : intersection.surfaces) {
                                IntersectableSurface surface = pSurface.first;
                                std::visit(overloaded{
                                    [&](auto &s) {
                                        s.get().clearMask();
                                        for(auto &pOtherIntersection : appState.intersectionSet) {
                                            if(pOtherIntersection.first == intersection.id)  continue;
                                            Intersection &otherIntersection = *pOtherIntersection.second;
                                            if(std::visit(overloaded{[&](auto &e){ return e.get().id == s.get().id;}}, otherIntersection.surfaces[0].first))
                                                appState.surfaceIntersection.addToMask(surface, otherIntersection, 0);
                                            if(std::visit(overloaded{[&](auto &e){ return e.get().id == s.get().id;}}, otherIntersection.surfaces[1].first))
                                                appState.surfaceIntersection.addToMask(surface, otherIntersection, 1);
                                        }
                                    }
                                }, surface);
                            }
                            appState.intersectionSet.erase(appState.intersectionSet.find(intersection.id));
                        }
                    }
                }

            }
            for(auto &patch : appState.patchC2Set) {
                bool patchChanged = false;
                for (int i = 0; i < patch.second->controlPoints.size(); i++)
                    if (patch.second->controlPoints[i].first == event.point.id) {
                        patchChanged = true;
                        patch.second->updatePoint(event.point, i);
                    }
                if (patchChanged) {
                    for(auto &pIntersection : appState.intersectionSet) {
                        Intersection &intersection = *pIntersection.second;
                        if(std::visit(overloaded{[&](auto &e){ return e.get().id == patch.first;}}, intersection.surfaces[0].first) ||
                            std::visit(overloaded{[&](auto &e){ return e.get().id == patch.first;}}, intersection.surfaces[1].first)) {
                            for(auto &pSurface : intersection.surfaces) {
                                IntersectableSurface surface = pSurface.first;
                                std::visit(overloaded{
                                    [&](auto &s) {
                                        s.get().clearMask();
                                        for(auto &pOtherIntersection : appState.intersectionSet) {
                                            if(pOtherIntersection.first == intersection.id)  continue;
                                            Intersection &otherIntersection = *pOtherIntersection.second;
                                            if(std::visit(overloaded{[&](auto &e){ return e.get().id == s.get().id;}}, otherIntersection.surfaces[0].first))
                                                appState.surfaceIntersection.addToMask(surface, otherIntersection, 0);
                                            if(std::visit(overloaded{[&](auto &e){ return e.get().id == s.get().id;}}, otherIntersection.surfaces[1].first))
                                                appState.surfaceIntersection.addToMask(surface, otherIntersection, 1);
                                        }
                                    }
                                }, surface);
                            }
                            appState.intersectionSet.erase(appState.intersectionSet.find(intersection.id));
                        }
                    }
                }
            }

            for(auto &gregory : appState.gregoryPatchSet)
                for(auto &sidePoints : gregory.second->patchSides)
                    for(auto &point : sidePoints)
                        if(point.pointId == event.point.id || point.outgoingBernstein[0] == event.point.id)
                            gregory.second->recalculate(appState.gregoryPatchCreator);

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
