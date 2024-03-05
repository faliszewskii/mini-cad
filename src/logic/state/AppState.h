//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPSTATE_H
#define OPENGL_SANDBOX_APPSTATE_H


#include <utility>
#include <vector>
#include <set>
#include "SelectionGroup.h"
#include "../algebra/Rect.h"
#include "imgui.h"
#include "../../../lib/imguizmo/ImGuizmo.h"
#include "../logger/Logger.h"
#include "../geometry/Torus.h"
#include "../concepts/has_id.h"
#include "../concepts/has_selected.h"
#include "../events/broadcaster/EventPublisher.h"
#include "../events/CreateTorusEvent.h"
#include "../events/CreatePointEvent.h"
#include "../events/SelectEntityEvent.h"
#include "EntityType.h"

class DebugOverlayModule;
class MenuBarModule;
class WorkspaceModule;
class LogPanelModule;
class GizmoModule;
class WireframeRenderModule;
class GridModule;
class VerticalStripedLineModule;
class CursorModule;
class CenterOfMassModule;

struct AppState {
    explicit AppState(Rect<int> viewport, int guiPanelLeftWidth);

    void runModules();

    std::unique_ptr<DebugOverlayModule> debugOverlayModule;
    std::unique_ptr<GizmoModule> gizmoModule;
    std::unique_ptr<LogPanelModule> logPanelModule;
    std::unique_ptr<MenuBarModule> menuBarModule;
    std::unique_ptr<WorkspaceModule> workspaceModule;
    std::unique_ptr<WireframeRenderModule> wireframeRendererModule;
    std::unique_ptr<GridModule> gridModule;
    std::unique_ptr<VerticalStripedLineModule> verticalStripedLineModule;
    std::unique_ptr<CursorModule> cursorModule;
    std::unique_ptr<CenterOfMassModule> centerOfMassModule;

    std::map<int, std::unique_ptr<Torus>> torusSet;
    std::map<int, std::unique_ptr<Point>> pointSet;

    std::map<int, EntityType> selectedEntities;


    EventPublisher<CreateTorusEvent, CreatePointEvent, SelectEntityEvent> eventPublisher;
//    EventDispatcher eventDispatcher;
    // TODO You can subscribe to a event described by some type with a lambda. You got a notification each time the event is triggered.
    // TODO Deleted sth event should be a type with the reference to the object or sth. This could be achieved with a template.
    // TODO System should be able to subscribe to it

    Logger logger;
    SelectionGroup selectionGroup;
    Camera camera;
    bool guiFocus;
    int guiPanelLeftWidth;
    std::pair<int, std::array<float, 100>> rollingMspf;
    bool vSync;
    glm::vec3 cursorPosition{};

//    glm::vec3 centerOfMass{0,1,0};
    Transformation centerOfMassTransformation;

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::UNIVERSAL;

    bool keyboardCtrlMode;
};

#endif //OPENGL_SANDBOX_APPSTATE_H
