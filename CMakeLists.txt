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


file(GLOB_RECURSE SOURCES src/*.cpp
                          src/platform/*.cpp
                          src/utils/*.cpp
                          src/world/*.cpp)

add_library(glad STATIC lib/glad/src/glad.c)

add_executable(VoxelEngine ${SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(VoxelEngine glfw OpenGL::GL glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")