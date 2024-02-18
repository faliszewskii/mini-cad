include_directories(../lib/glad/include)

set(CMAKE_CXX_STANDARD 23)

set(IMGUI
        ../lib/imgui/imgui_draw.cpp
        ../lib/imgui/imgui_tables.cpp
        ../lib/imgui/imgui_widgets.cpp
        ../lib/imgui/imgui_demo.cpp
        ../lib/imgui/imconfig.h
        ../lib/imgui/imgui.cpp
        ../lib/imgui/imgui.h
        ../lib/imgui/imgui_internal.h
        ../lib/imgui/imstb_rectpack.h
        ../lib/imgui/imstb_textedit.h
        ../lib/imgui/imstb_truetype.h
        ../lib/imgui/backends/imgui_impl_glfw.cpp
        ../lib/imgui/backends/imgui_impl_glfw.h
        ../lib/imgui/backends/imgui_impl_opengl3.cpp
        ../lib/imgui/backends/imgui_impl_opengl3.h
        ../lib/imgui/misc/cpp/imgui_stdlib.cpp
        ../lib/imgui/misc/cpp/imgui_stdlib.h
        ../lib/imguizmo/ImGuizmo.cpp
        ../lib/imguizmo/ImGuizmo.h
        ../lib/imgui-node-editor/imgui_node_editor.h
        ../lib/imgui-node-editor/imgui_node_editor.cpp
        ../lib/imgui-node-editor/crude_json.cpp
        ../lib/imgui-node-editor/crude_json.h
        ../lib/imgui-node-editor/imgui_bezier_math.h
        ../lib/imgui-node-editor/imgui_canvas.h
        ../lib/imgui-node-editor/imgui_canvas.cpp
        ../lib/imgui-node-editor/imgui_node_editor_api.cpp
)

set(INCL
        ../lib/glad/glad.c
        presentation/scene/nodes/shader/Shader.h
        logic/opengl/OpenGLInstance.cpp
        logic/opengl/OpenGLInstance.h
        logic/io/IOUtils.cpp
        logic/io/IOUtils.h
        logic/importer/AssetImporter.cpp
        logic/importer/AssetImporter.h
        logic/generator/ModelGenerator.cpp
        logic/generator/ModelGenerator.h
        logic/algebra/AlgebraUtils.cpp
        logic/algebra/AlgebraUtils.h
        presentation/gui/GUI.cpp
        presentation/gui/GUI.h
        logic/state/AppState.h
        ../lib/glad/glad_glfw.h
        ../lib/uuid/uuid.h
        presentation/scene/tree/SceneNode.h
        presentation/scene/visitor/SceneNodeVisitor.h
        presentation/rendering/renderVisitor/RenderSceneVisitor.cpp
        presentation/rendering/renderVisitor/RenderSceneVisitor.h
        presentation/scene/nodes/light/Light.cpp
        presentation/scene/nodes/light/Light.h
        presentation/scene/nodes/transformation/Transformation.cpp
        presentation/scene/nodes/transformation/Transformation.h
        presentation/scene/nodes/mesh/Mesh.cpp
        presentation/scene/nodes/mesh/Mesh.h
        presentation/scene/nodes/camera/Camera.cpp
        presentation/scene/nodes/camera/Camera.h
        presentation/gui/tree/TreeViewVisitor.cpp
        presentation/gui/tree/TreeViewVisitor.h
        logic/input/InputHandler.cpp
        logic/input/InputHandler.h
        presentation/gui/node/NodeDetailsVisitor.cpp
        presentation/gui/node/NodeDetailsVisitor.h
        presentation/scene/nodes/material/Material.cpp
        presentation/scene/nodes/material/Material.h
        presentation/scene/tree/Bindable.h
        presentation/scene/nodes/frameBuffer/FrameBuffer.cpp
        presentation/scene/nodes/frameBuffer/FrameBuffer.h
        presentation/gui/editor/EditorNodeVisitor.cpp
        presentation/gui/editor/EditorNodeVisitor.h
        presentation/scene/steps/RenderingStep.cpp
        presentation/scene/steps/RenderingStep.h
        presentation/scene/visitor/StepVisitor.h
        presentation/rendering/renderVisitor/ExecuteStepVisitor.cpp
        presentation/rendering/renderVisitor/ExecuteStepVisitor.h
        presentation/gui/tree/StepTreeViewVisitor.cpp
        presentation/gui/tree/StepTreeViewVisitor.h
        presentation/gui/node/StepParametersVisitor.cpp
        presentation/gui/node/StepParametersVisitor.h
        logic/Scene.h
        presentation/modules/Module.cpp
        presentation/modules/Module.h
        presentation/modules/renderers/PhongRenderModule.h
        logic/algebra/Rect.h
        presentation/modules/renderers/RenderHelpers.h
        presentation/modules/renderers/RandomRenderModule.h
        presentation/modules/renderers/NormalRenderModule.h
        presentation/modules/gui/WorkspaceModule.h
        presentation/modules/gui/MenuBarModule.h
        presentation/modules/gui/LogPanelModule.h
        presentation/modules/gui/DebugOverlayModule.h
        presentation/modules/gui/GizmoModule.h
        logic/utils/VariantUtil.h
        logic/state/TransformTree.h
        logic/state/SelectionGroup.h
        presentation/modules/gui/workspaces/TransformTreeWorkspace.h
        presentation/modules/gui/workspaces/MaterialsWorkspace.h
        presentation/modules/gui/workspaces/LightsWorkspace.h
        presentation/modules/gui/workspaces/CamerasWorkspace.h
        presentation/modules/gui/workspaces/DebugOverlayModuleWorkspace.h
        presentation/modules/gui/workspaces/GizmoModuleWorkspace.h
        presentation/modules/gui/workspaces/LogPanelModuleWorkspace.h
        presentation/modules/gui/workspaces/MenuBarModuleWorkspace.h
        presentation/modules/gui/workspaces/PhongRendererModuleWorkspace.h
        presentation/modules/gui/workspaces/NormalRendererModuleWorkspace.h
        presentation/modules/gui/workspaces/RandomRendererModule.h
        presentation/modules/gui/workspaces/ModulesWorkspaceHelpers.h
        logic/state/AppState.cpp
        logic/importer/ImportResult.h
        logic/importer/ImportResult.cpp
        presentation/modules/MeshGenerationModule.h
        logic/generator/MeshGenerator.h
        logic/generator/ParametrisedMeshGenerator.h
        logic/generator/MeshGeneratorHelpers.h
        logic/generator/ParametrisedMeshGenerator.cpp
        logic/generator/HyperbolicParaboloidGenerator.h
        presentation/modules/gui/workspaces/MeshGeneratorsWorkspace.h
        logic/generator/TorusGenerator.h
)

add_executable(opengl-sandbox main.cpp
        ${IMGUI}
        ${INCL}
)

set(CFLAGS -std=c++23 -fmodules-ts)
set(LDFLAGS -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp -luuid nfd)

target_include_directories(opengl-sandbox PRIVATE ../lib/imgui)
target_compile_options(opengl-sandbox PUBLIC ${CFLAGS})
target_link_libraries(opengl-sandbox ${LDFLAGS})