cmake_minimum_required(VERSION 3.27)
project(opengl-sandbox)
add_subdirectory(src)

find_package(glm CONFIG REQUIRED)
add_subdirectory(lib/glfw)

# add subdirectory with path to folder containing Scene-Serializer CMakeList.txt, this should include it in build process
add_subdirectory(lib/Scene-Serializer/cpp/Serializer)

# add Serializer library to project libraries; path to Serializer subproject built lib file
target_link_libraries(opengl-sandbox MGSerializer glm::glm)

# add header files to include search path
target_include_directories(opengl-sandbox PUBLIC lib/Scene-Serializer/cpp/Serializer/Serializer)

add_subdirectory(lib/nativefiledialog-extended)
