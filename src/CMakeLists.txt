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
)

add_executable(opengl-sandbox main.cpp
        ${IMGUI}
        ../lib/glad/glad.c
        presentation/rendering/shader/shader.h
        presentation/camera/camera.cpp
        presentation/camera/camera.h
        logic/opengl/OpenGLInstance.cpp
        logic/opengl/OpenGLInstance.h
        logic/io/IOUtils.cpp
        logic/io/IOUtils.h
        logic/importer/AssetImporter.cpp
        logic/importer/AssetImporter.h
        presentation/rendering/model/Mesh.cpp
        presentation/rendering/model/Mesh.h
        presentation/rendering/model/Model.cpp
        presentation/rendering/model/Model.h
        presentation/rendering/model/ModelNode.cpp
        presentation/rendering/model/ModelNode.h
)

set(CFLAGS -std=c++23 -fmodules-ts)
set(LDFLAGS -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp)

target_include_directories(opengl-sandbox PRIVATE ../lib/imgui)
target_compile_options(opengl-sandbox PUBLIC ${CFLAGS})
target_link_libraries(opengl-sandbox ${LDFLAGS})