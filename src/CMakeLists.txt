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
        logic/entities/shader/Shader.h
        logic/opengl/OpenGLInstance.cpp
        logic/opengl/OpenGLInstance.h
        logic/io/IOUtils.cpp
        logic/io/IOUtils.h
        logic/importer/AssetImporter.cpp
        logic/importer/AssetImporter.h
        logic/algebra/AlgebraUtils.cpp
        logic/algebra/AlgebraUtils.h
        presentation/gui/GUI.cpp
        presentation/gui/GUI.h
        logic/state/AppState.h
        ../lib/glad/glad_glfw.h
        ../lib/uuid/uuid.h
        logic/entities/SceneNode.h
        logic/entities/light/Light.cpp
        logic/entities/light/Light.h
        logic/entities/transformation/Transformation.cpp
        logic/entities/transformation/Transformation.h
        logic/entities/mesh/Mesh.h
        logic/entities/camera/Camera.cpp
        logic/entities/camera/Camera.h
        logic/input/InputHandler.cpp
        logic/input/InputHandler.h
        logic/entities/material/Material.cpp
        logic/entities/material/Material.h
        logic/entities/frameBuffer/FrameBuffer.cpp
        logic/entities/frameBuffer/FrameBuffer.h
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
        presentation/gui/workspaces/TransformTreeWorkspace.h
        presentation/gui/workspaces/MaterialsWorkspace.h
        presentation/gui/workspaces/LightsWorkspace.h
        presentation/gui/workspaces/CamerasWorkspace.h
        presentation/gui/workspaces/DebugOverlayModuleWorkspace.h
        presentation/gui/workspaces/GizmoModuleWorkspace.h
        presentation/gui/workspaces/LogPanelModuleWorkspace.h
        presentation/gui/workspaces/MenuBarModuleWorkspace.h
        presentation/gui/workspaces/PhongRendererModuleWorkspace.h
        presentation/gui/workspaces/NormalRendererModuleWorkspace.h
        presentation/gui/workspaces/RandomRendererModuleWorkspace.h
        presentation/gui/workspaces/ModulesWorkspaceHelpers.h
        logic/state/AppState.cpp
        logic/importer/ImportResult.h
        logic/importer/ImportResult.cpp
        logic/generator/MeshGenerator.h
        logic/generator/ParametrisedMeshGenerator.h
        logic/generator/MeshGeneratorHelpers.h
        logic/generator/ParametrisedMeshGenerator.cpp
        logic/generator/HyperbolicParaboloidGenerator.h
        presentation/gui/workspaces/MeshGeneratorsWorkspace.h
        logic/generator/TorusGeneratorManual.h
        logic/generator/SphereGeneratorManual.h
        presentation/modules/renderers/GridModule.h
        logic/logger/Logger.h
        presentation/gui/workspaces/GridModuleWorkspace.h
        logic/generator/NormOneGenerator.h
        presentation/modules/renderers/WireframeRenderModule.h
        presentation/gui/workspaces/WireframeRenderModuleWorkspace.h
        presentation/modules/renderers/RayCastingModule.h
        presentation/gui/workspaces/RayCastingModuleWorkspace.h
        logic/generator/CircleGenerator.h
        logic/generator/CylinderGenerator.h
        logic/generator/SphereGenerator.h
        logic/generator/TorusGenerator.h
        logic/geometry/Point.h
        presentation/gui/workspaces/PointWorkspace.h
        presentation/modules/renderers/VerticalStripedLineModule.h
        logic/vertices/is_vertex.h
        logic/vertices/Vertex.h
        logic/vertices/EmptyVertex.h
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