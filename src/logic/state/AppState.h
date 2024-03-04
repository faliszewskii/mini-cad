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

class DebugOverlayModule;
class MenuBarModule;
class WorkspaceModule;
class LogPanelModule;
class GizmoModule;
class WireframeRenderModule;
class GridModule;
class VerticalStripedLineModule;

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

    std::map<int, std::unique_ptr<Torus>> torusSet;
    std::map<int, std::unique_ptr<Point>> pointSet;

    class SelectedEntities {
        template<typename ...RefT> using ReferenceVariant = std::variant<std::reference_wrapper<RefT>...>;
        template<typename ...T> requires (has_id<T> && ...) && (has_selected<T> && ...) using EntityTypeTemp = ReferenceVariant<T...>;
        using EntityType = EntityTypeTemp<Torus, Point>;

    public:
        std::map<int, EntityType> set;

        auto getEntities() {
            return std::views::values(set);
        }

        template<typename T>
        void add(AppState &appState, T &entity) {
            if (!appState.keyboardCtrlMode) clear();
            set.emplace(entity.id, entity);
            entity.selected = true;
        }

        void clear() {
            for(auto &el : set) std::visit([](auto &el){el.get().selected = false;}, el.second);
            set.clear();
        }
    } selectedEntities;


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

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::UNIVERSAL;

    bool keyboardCtrlMode;
};

#endif //OPENGL_SANDBOX_APPSTATE_H
