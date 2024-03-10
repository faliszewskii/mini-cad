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
        logic/algebra/AlgebraUtils.cpp
        logic/algebra/AlgebraUtils.h
        presentation/gui/GUI.cpp
        presentation/gui/GUI.h
        logic/state/AppState.h
        ../lib/glad/glad_glfw.h
        ../lib/uuid/uuid.h
        logic/entities/SceneNode.h
        logic/entities/transformation/Transformation.cpp
        logic/entities/transformation/Transformation.h
        logic/entities/mesh/Mesh.h
        logic/entities/camera/Camera.cpp
        logic/entities/camera/Camera.h
        logic/input/InputHandler.cpp
        logic/input/InputHandler.h
        logic/algebra/Rect.h
        presentation/modules/renderers/RenderHelpers.h
        presentation/modules/gui/WorkspaceModule.h
        presentation/modules/gui/MenuBarModule.h
        presentation/modules/gui/LogPanelModule.h
        presentation/modules/gui/DebugOverlayModule.h
        presentation/modules/gui/GizmoModule.h
        logic/utils/VariantUtil.h
        presentation/gui/workspaces/CameraWorkspace.h
        presentation/gui/workspaces/DebugOverlayModuleWorkspace.h
        presentation/gui/workspaces/GizmoModuleWorkspace.h
        presentation/gui/workspaces/LogPanelModuleWorkspace.h
        presentation/gui/workspaces/MenuBarModuleWorkspace.h
        logic/state/AppState.cpp
        logic/geometry/Torus.h
        presentation/modules/renderers/GridModule.h
        logic/logger/Logger.h
        presentation/gui/workspaces/GridModuleWorkspace.h
        presentation/modules/renderers/WireframeRenderModule.h
        presentation/gui/workspaces/WireframeRenderModuleWorkspace.h
        logic/geometry/Point.h
        presentation/gui/workspaces/PointWorkspace.h
        presentation/modules/renderers/VerticalStripedLineModule.h
        logic/vertices/is_vertex.h
        logic/vertices/Vertex.h
        logic/vertices/EmptyVertex.h
        logic/geometry/GeometryHelpers.h
        presentation/gui/workspaces/EntityListWorkspace.h
        logic/concepts/has_name.h
        logic/ids/IdCounter.h
        logic/concepts/has_id.h
        presentation/gui/workspaces/CursorWorkspace.h
        presentation/modules/renderers/CenterOfMassModule.h
        logic/concepts/has_selected.h
        presentation/modules/renderers/CursorModule.h
        logic/events/broadcaster/EventPublisher.h
        logic/events/CreateTorusEvent.h
        logic/handlers/ObjectCreationHandler.h
        logic/events/CreatePointEvent.h
        logic/events/EntitySelectionChangedEvent.h
        logic/events/SelectEntityEvent.h
        logic/state/EntityType.h
        logic/geometry/BezierC0.h
        logic/events/CreateBezierC0Event.h
        logic/vertices/PositionVertex.h
        logic/events/PointMovedEvent.h
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