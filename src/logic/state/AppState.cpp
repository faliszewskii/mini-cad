//
// Created by faliszewskii on 17.02.24.
//

#include "../../presentation/modules/renderers/CenterOfMassModule.h"
#include "../../presentation/modules/renderers/CursorModule.h"
#include "../../presentation/modules/renderers/VerticalStripedLineModule.h"
#include "../../presentation/modules/renderers/GridModule.h"
#include "AppState.h"
#include "../../presentation/modules/gui/DebugOverlayModule.h"
#include "../../presentation/modules/gui/MenuBarModule.h"
#include "../../presentation/modules/gui/WorkspaceModule.h"

AppState::AppState(Rect<int> viewport, int guiPanelLeftWidth) :
            camera(viewport.width, viewport.height, CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f)),
            guiFocus(true),
            vSync(true),
            keyboardCtrlMode(false),
            guiPanelLeftWidth(guiPanelLeftWidth),
            menuBarModule(std::make_unique<MenuBarModule>()),
            logPanelModule(std::make_unique<LogPanelModule>()),
            debugOverlayModule(std::make_unique<DebugOverlayModule>(guiPanelLeftWidth)),
            gizmoModule(std::make_unique<GizmoModule>(guiPanelLeftWidth)),
            workspaceModule(std::make_unique<WorkspaceModule>(guiPanelLeftWidth)),
            wireframeRendererModule(std::make_unique<WireframeRenderModule>(guiPanelLeftWidth)),
            gridModule(new GridModule(guiPanelLeftWidth)),
            verticalStripedLineModule(std::make_unique<VerticalStripedLineModule>(guiPanelLeftWidth)),
            cursorModule(std::make_unique<CursorModule>(guiPanelLeftWidth)),
            centerOfMassModule(std::make_unique<CenterOfMassModule>(guiPanelLeftWidth)) {
        glfwSwapInterval(vSync);

        eventPublisher.subscribe([&](const CreateTorusEvent &event){
            auto torus = std::make_unique<Torus>(event.position);
            this->torusSet.emplace(torus->id, std::move(torus));
        });
        eventPublisher.subscribe([&](const CreatePointEvent &event) {
            auto point = std::make_unique<Point>(event.position);
            auto result = this->pointSet.emplace(point->id, std::move(point));
            if(!selectedEntities.empty()) { // TODO Move this to entirely different handler for bezier that is called on PointCreatedEvent.
                auto &last = (selectedEntities.rbegin())->second;
                if(holds_alternative<std::reference_wrapper<BezierC0>>(last)) {
                    BezierC0 &bezier = std::get<std::reference_wrapper<BezierC0>>(last);
                    bezier.addPoint(*result.first->second);
                }
            }
        });
        eventPublisher.subscribe([&](const CreateBezierC0Event &event) {
            auto bezier = std::make_unique<BezierC0>();
            auto result = this->bezierC0Set.emplace(bezier->id, std::move(bezier));

            // Add all currently selected points;
            for(auto &el : selectedEntities) { // TODO Consider separating selected entities into object types.
                if(holds_alternative<std::reference_wrapper<Point>>(el.second)) {
                    Point &point = std::get<std::reference_wrapper<Point>>(el.second);
                    result.first->second->addPoint(point);
                }
            }
        });
        eventPublisher.subscribe([&](const SelectEntityEvent &event) {
            auto &set = this->selectedEntities;
            if (!this->keyboardCtrlMode) {
                for(auto &el : set) std::visit([](auto &el){el.get().selected = false;}, el.second);
                set.clear();
            }
            std::visit(overloaded{
                [&](auto &el){
                    auto &entity = el.get();
                    set.emplace(entity.id, entity);
                    entity.selected = true;
                }
            }, event.selected);

            // Center of mass
            if(set.size() > 1) {
                glm::vec3 center = glm::vec3();
                for(auto &el : set) {
                    std::visit(overloaded{
                            [&](Torus &torus) { center += torus.transform.translation; },
                            [&](Point &point) { center += point.position; },
                            [&](BezierC0 &bezier) { /*TODO*/ }
                    }, el.second);
                }
                center /= set.size();
                centerOfMassTransformation = Transformation{center};
            }
        });
        eventPublisher.subscribe([&](const PointMovedEvent &event) {
            // TODO There should be a data structure that would allow to query for Bezier that is build from given point.
            for(auto &bezier : this->bezierC0Set) {
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.point.id)
                        bezier.second->updatePoint(event.point, i);
            }
        });
        eventPublisher.subscribe([&](const PointDeletedEvent &event) {
            // TODO There should be a data structure that would allow to query for Bezier that is build from given point.
            for(auto &bezier : this->bezierC0Set) {
                for(int i = 0; i < bezier.second->controlPoints.size(); i++)
                    if(bezier.second->controlPoints[i].first == event.id)
                        bezier.second->removePoint(i);
            }
        });
    }

void AppState::runModules() {
    wireframeRendererModule->run(*this);
    verticalStripedLineModule->run(*this);
    cursorModule->run(*this);
    centerOfMassModule->run(*this);

    // Grid has to be at the end to maintain transparency.
    gridModule->run(*this);

    // UI
    workspaceModule->run(*this);
    menuBarModule->run(*this);
    debugOverlayModule->run(*this);
    logPanelModule->run(*this);
    gizmoModule->run(*this);
}
