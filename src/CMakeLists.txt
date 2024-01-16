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

add_executable(opengl-sandbox main.cpp
        ${IMGUI}
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
        logic/ApplicationState.h
        ../lib/glad/glad_glfw.h
        ../lib/uuid/uuid.h
        presentation/scene/tree/SceneNode.h
        presentation/scene/tree/SceneTreeNode.h
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
        presentation/scene/tree/SceneTreeNode.cpp
        presentation/gui/properties/PropertyViewVisitor.cpp
        presentation/gui/properties/PropertyViewVisitor.h
        logic/input/InputHandler.cpp
        logic/input/InputHandler.h
        presentation/properties/transformation/TransformationProperty.cpp
        presentation/properties/transformation/TransformationProperty.h
        presentation/properties/Property.cpp
        presentation/properties/Property.h
        presentation/properties/visitor/PropertyVisitor.cpp
        presentation/properties/visitor/PropertyVisitor.h
        presentation/gui/node/NodeDetailsVisitor.cpp
        presentation/gui/node/NodeDetailsVisitor.h
        presentation/properties/light/LightTypeProperty.h
        presentation/properties/light/PointLightProperty.cpp
        presentation/properties/light/PointLightProperty.h
        presentation/scene/nodes/material/Material.cpp
        presentation/scene/nodes/material/Material.h
        presentation/scene/tree/Bindable.h
        presentation/scene/nodes/frameBuffer/FrameBuffer.cpp
        presentation/scene/nodes/frameBuffer/FrameBuffer.h
)

set(CFLAGS -std=c++23 -fmodules-ts)
set(LDFLAGS -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp -luuid nfd)

target_include_directories(opengl-sandbox PRIVATE ../lib/imgui)
target_compile_options(opengl-sandbox PUBLIC ${CFLAGS})
target_link_libraries(opengl-sandbox ${LDFLAGS})