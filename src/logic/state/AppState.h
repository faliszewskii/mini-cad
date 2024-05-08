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
#include "../events/create/CreateTorusEvent.h"
#include "../events/create/CreatePointEvent.h"
#include "../events/selection/SelectEntityEvent.h"
#include "EntityType.h"
#include "../entities/camera/Camera.h"
#include "../geometry/BezierC0.h"
#include "../events/create/CreateBezierC0Event.h"
#include "../events/point/PointMovedEvent.h"
#include "../events/point/PointDeletedEvent.h"
#include "../events/selection/SelectionChangedEvent.h"
#include "../geometry/BezierC2.h"
#include "../events/create/CreateBezierC2Event.h"
#include "../events/input/MouseButtonEvent.h"
#include "../events/input/MouseMovedEvent.h"
#include "../events/input/MouseScrolledEvent.h"
#include "../events/input/KeyEvent.h"
#include "../events/create/CreateInterpolatedC2Event.h"
#include "../creator/BezierPatchCreator.h"
#include "../events/create/CreateBezierPatch.h"

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
    std::map<int, std::unique_ptr<InterpolatedC2>> interpolatedC2Set;
    std::map<int, std::unique_ptr<PatchC0>> patchC0Set;
    std::map<int, std::unique_ptr<PatchC2>> patchC2Set;

    std::vector<std::pair<int, EntityType>> selectedEntities;
    std::vector<std::pair<int, EntityType>> selectionContext;

    int bezierPatchGridWidth;
    int bezierPatchGridLength;
    BezierPatchCreator bezierPatchCreator;

    EventPublisher<CreateTorusEvent, CreatePointEvent, CreateBezierC0Event, SelectEntityEvent, PointMovedEvent,
        PointDeletedEvent, SelectionChangedEvent, CreateBezierC2Event, MouseButtonEvent, MouseMovedEvent,
        MouseScrolledEvent, KeyEvent, CreateInterpolatedC2Event, CreateBezierPatch> eventPublisher;

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
    bool bezierCreatorOpen;
};

#endif //OPENGL_SANDBOX_APPSTATE_H
