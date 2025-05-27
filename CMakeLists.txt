cmake_minimum_required(VERSION 3.27)
project(mini-cad)
add_subdirectory(src)

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

add_subdirectory(lib/glfw)

# add subdirectory with path to folder containing Scene-Serializer CMakeList.txt, this should include it in build process
add_subdirectory(lib/Scene-Serializer/cpp/Serializer)

# add Serializer library to project libraries; path to Serializer subproject built lib file
target_link_libraries(mini-cad MGSerializer glm::glm)

# add header files to include search path
target_include_directories(mini-cad PUBLIC lib/Scene-Serializer/cpp/Serializer/Serializer)

add_subdirectory(lib/nativefiledialog-extended)
