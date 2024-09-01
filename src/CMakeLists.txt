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
)

set(INCL
        ../lib/glad/glad.c
        logic/entities/shader/Shader.h
        logic/opengl/OpenGLInstance.cpp
        logic/opengl/OpenGLInstance.h
        logic/io/IOUtils.cpp
        logic/io/IOUtils.h
        logic/math/algebra/AlgebraUtils.cpp
        logic/math/algebra/AlgebraUtils.h
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
        logic/math/algebra/Rect.h
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
        logic/events/create/CreateTorusEvent.h
        logic/events/create/CreatePointEvent.h
        logic/events/selection/SelectEntityEvent.h
        logic/state/EntityType.h
        logic/geometry/BezierC0.h
        logic/events/create/CreateBezierC0Event.h
        logic/vertices/PositionVertex.h
        logic/events/point/PointMovedEvent.h
        logic/events/point/PointDeletedEvent.h
        logic/events/selection/SelectionChangedEvent.h
        logic/geometry/BezierC2.h
        logic/events/create/CreateBezierC2Event.h
        logic/events/create/CreateEventsHandler.h
        logic/events/selection/SelectionEventsHandler.h
        logic/events/point/PointEventsHandler.h
        logic/events/input/MouseButtonEvent.h
        logic/events/input/InputEventsHandler.h
        logic/events/input/MouseMovedEvent.h
        logic/events/input/MouseScrolledEvent.h
        logic/events/input/KeyEvent.h
        logic/geometry/InterpolatedC2.h
        logic/events/create/CreateInterpolatedC2Event.h
        logic/creator/BeizerPatchCreator.cpp
        logic/creator/BezierPatchCreator.h
        presentation/gui/creator/BezierPatchCreatorUI.cpp
        presentation/gui/creator/BezierPatchCreatorUI.h
        logic/events/create/CreateBezierPatch.h
        logic/creator/BezierPatchCreatorParams.h
        logic/geometry/PatchC0.h
        logic/geometry/PatchC2.h
        logic/serializer/Serializer.cpp
        logic/serializer/Serializer.h
        logic/events/selection/ResetSelectionEvent.h
        logic/creator/GregoryPatchCreator.cpp
        logic/creator/GregoryPatchCreator.h
        presentation/gui/creator/GregoryPatchCreatorUI.cpp
        presentation/gui/creator/GregoryPatchCreatorUI.h
        logic/math/graph/Graph.h
        logic/math/graph/GregoryPatchGraphVertex.h
        logic/geometry/GregoryPatch.h
        logic/events/create/CreateGregoryPatchEvent.h
        logic/intersections/SurfaceIntersection.h
        logic/intersections/ParametricSurface.h
        logic/intersections/Intersectable.h
        logic/texture/Texture.h
        logic/texture/Texture.cpp
        logic/intersections/IntersectableSurface.h
        logic/intersections/Intersection.h
        logic/events/create/CreateIntersectionEvent.h
        logic/algorithms/BresenhamAlgorithm.cpp
        logic/algorithms/BresenhamAlgorithm.h
        logic/algorithms/FloodFill.cpp
        logic/algorithms/FloodFill.h
)

add_executable(opengl-sandbox main.cpp
        ${IMGUI}
        ${INCL}
)

set(CFLAGS -std=c++23 -fmodules-ts#[[ -O3]])
set(LDFLAGS -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp -luuid nfd)

target_include_directories(opengl-sandbox PRIVATE ../lib/imgui ../lib/GLFW)
target_compile_options(opengl-sandbox PUBLIC ${CFLAGS})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(opengl-sandbox ${LDFLAGS})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")        # Windows
    find_package(OpenGL REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(opengl-sandbox
            glfw
            nfd
            ${OPENGL_LIBRARIES}
    )
endif()