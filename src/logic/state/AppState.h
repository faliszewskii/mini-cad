//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_APPSTATE_H
#define OPENGL_SANDBOX_APPSTATE_H


#include <utility>
#include <vector>
#include <set>
#include "../importer/AssetImporter.h"
#include "../../presentation/scene/nodes/transformation/Transformation.h"
#include "../../presentation/scene/nodes/camera/Camera.h"
#include "../../presentation/scene/tree/StepTree.h"
#include "../generator/ModelGenerator.h"
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

struct AppState {
    explicit AppState(Rect<int> viewport, int guiPanelLeftWidth);

    void runModules();

    AssetImporter assetImporter;
    // TODO ModelGenerator ?
    // TODO Logger
    Scene scene;

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

    Logger logger;
    SelectionGroup selectionGroup;
    std::optional<std::reference_wrapper<Camera>> currentCamera;
    bool guiFocus;
    std::pair<int, std::array<float, 100>> rollingMspf;
    bool vSync;

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::UNIVERSAL;

    // TODO Delete
    std::optional<std::reference_wrapper<SceneNode>> selectedNode;
    std::optional<std::reference_wrapper<RenderingStep>> selectedStep;
    std::optional<std::reference_wrapper<SceneNode>> mainFrameBufferNode;
    std::vector<std::unique_ptr<SceneNode>> allNodes;

};

#endif //OPENGL_SANDBOX_APPSTATE_H
