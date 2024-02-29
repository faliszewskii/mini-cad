//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPSTATE_H
#define OPENGL_SANDBOX_APPSTATE_H


#include <utility>
#include <vector>
#include <set>
#include "../importer/AssetImporter.h"
#include "TransformTree.h"
#include "SelectionGroup.h"
#include "../algebra/Rect.h"
#include "imgui.h"
#include "../../../lib/imguizmo/ImGuizmo.h"
#include "../generator/MeshGenerator.h"
#include "../logger/Logger.h"

class DebugOverlayModule;
class MenuBarModule;
class WorkspaceModule;
class LogPanelModule;
class GizmoModule;
class NormalRenderModule;
class PhongRenderModule;
class RandomRenderModule;
class WireframeRenderModule;
class GridModule;
class RayCastingModule;
class VerticalStripedLineModule;

struct AppState {
    explicit AppState(Rect<int> viewport, int guiPanelLeftWidth);

    void runModules();

    AssetImporter assetImporter;

    std::vector<Light> lights;
    std::vector<Camera> cameras;
    std::vector<std::unique_ptr<Material>> materials; // TODO Maybe map with name string as a key? Think about unique names instead of uuids.
    TransformTree transformTree;
    std::vector<std::unique_ptr<MeshGenerator>> meshGenerators;
    // TODO framebuffers
    // TODO textures

    std::unique_ptr<DebugOverlayModule> debugOverlayModule;
    std::unique_ptr<GizmoModule> gizmoModule;
    std::unique_ptr<LogPanelModule> logPanelModule;
    std::unique_ptr<MenuBarModule> menuBarModule;
    std::unique_ptr<WorkspaceModule> workspaceModule;
    std::unique_ptr<NormalRenderModule> normalRendererModule;
    std::unique_ptr<PhongRenderModule> phongRendererModule;
    std::unique_ptr<RandomRenderModule> randomRendererModule;
    std::unique_ptr<WireframeRenderModule> wireframeRendererModule;
    std::unique_ptr<GridModule> gridModule;
    std::unique_ptr<RayCastingModule> rayCastingModule;
    std::unique_ptr<VerticalStripedLineModule> verticalStripedLineModule;

    Logger logger;
    SelectionGroup selectionGroup;
    std::optional<std::reference_wrapper<Camera>> currentCamera;
    bool guiFocus;
    std::pair<int, std::array<float, 100>> rollingMspf;
    bool vSync;
    glm::vec3 cursorPosition{};

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::UNIVERSAL;
};

#endif //OPENGL_SANDBOX_APPSTATE_H
