cmake_minimum_required(VERSION 3.28)

add_executable(Test_Explore)
target_sources(Test_Explore PRIVATE Explore.cpp)
target_compile_features(Test_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_Explore PRIVATE
    Eqx_Std Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Quad/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Quad/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Quad/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Quad/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Quad/Fragment.glsl
    COPYONLY)
