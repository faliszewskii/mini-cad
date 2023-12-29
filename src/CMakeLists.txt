include_directories(../include)

set(CMAKE_CXX_STANDARD 23)

add_executable(opengl-sandbox main.cpp
        glad.c
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

target_compile_options(opengl-sandbox PUBLIC ${CFLAGS})
target_link_libraries(opengl-sandbox ${LDFLAGS})