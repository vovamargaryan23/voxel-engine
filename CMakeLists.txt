cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(VoxelEngine VERSION 1.0)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm/glm)

include_directories(
    "include"
    "lib/glfw/include"
    "lib/glad/include"
    "lib/glm"
    "lib/noise")

set(SOURCES
    src/main.cpp
    src/engine.cpp
    src/platform/camera.cpp
    src/platform/renderer.cpp
    src/platform/window.cpp
    src/utils/input_manager.cpp
    src/utils/shader.cpp
    src/world/chunk.cpp
    src/world/world_generator.cpp
)

add_library(glad STATIC lib/glad/src/glad.c)

add_executable(VoxelEngine ${SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(VoxelEngine glfw OpenGL::GL glad)

target_compile_definitions(VoxelEngine PRIVATE
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
)

target_compile_options(VoxelEngine PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
)
