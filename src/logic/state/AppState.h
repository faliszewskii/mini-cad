//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPSTATE_H
#define OPENGL_SANDBOX_APPSTATE_H


#include <utility>
#include <vector>
#include <set>
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
#include "../entities/camera/Camera.h"
#include "../geometry/BezierC0.h"
#include "../events/CreateBezierC0Event.h"
#include "../events/PointMovedEvent.h"
#include "../events/PointDeletedEvent.h"
#include "../events/SelectionChangedEvent.h"
#include "../geometry/BezierC2.h"
#include "../events/CreateBezierC2Event.h"

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
    std::map<int, std::unique_ptr<BezierC0>> bezierC0Set;
    std::map<int, std::unique_ptr<BezierC2>> bezierC2Set;

    std::vector<std::pair<int, EntityType>> selectedEntities;
    std::map<int, std::reference_wrapper<Point>> selectedBezierPoints;


    EventPublisher<CreateTorusEvent, CreatePointEvent, CreateBezierC0Event, SelectEntityEvent, PointMovedEvent,
        PointDeletedEvent, SelectionChangedEvent, CreateBezierC2Event> eventPublisher;

    Logger logger;
    Camera camera;
    bool guiFocus;
    int guiPanelLeftWidth;
    std::pair<int, std::array<float, 100>> rollingMspf;
    bool vSync;
    glm::vec3 cursorPosition{};

    Transformation centerOfMassTransformation;

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::UNIVERSAL;

    bool keyboardCtrlMode;
};

#endif //OPENGL_SANDBOX_APPSTATE_H
