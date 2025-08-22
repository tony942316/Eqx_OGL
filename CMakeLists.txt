cmake_minimum_required(VERSION 3.28)

project(Eqx_OGL
    VERSION 0.1.0.0
    DESCRIPTION "C++20 OpenGL Vertex Rendering"
    HOMEPAGE_URL "https://github.com/tony942316/Eqx_OGL"
    LANGUAGES CXX)

include(FetchContent)

add_subdirectory(OGL/)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(Tests/)
endif()
